/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultTask.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:46:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/16 05:45:24 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/DefaultTask.hpp"
#include "Cycle.hpp"
#include "HttpException.hpp"
#include "utility.hpp"

#include <ftpp/algorithm.hpp>
#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/format/Format.hpp>
#include <ftpp/html/html.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/macros.hpp>
#include <ftpp/pathlib/pathlib.hpp>
#include <ftpp/string/string.hpp>
#include <ftpp/subprocess/Subprocess.hpp>

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

DefaultTask::DefaultTask(Connection::Cycle &cycle,
                         LocationDefault const &location)
    : Task(cycle), _location(location), _cgiManager(NULL), _status(-1) {
  Request const &request = cycle.getRequest();
  ftpp::normpath(_location.getRoot() + request.path).swap(_path);
  if (!ftpp::starts_with(_path, _location.getRoot()))
    _status = 403;
  else {
    try {
      LocationDefault::Cgis const &cgis = _location.getCgis();
      for (LocationDefault::Cgis::const_iterator it = cgis.begin();
           it != cgis.end(); ++it) {
        if (ftpp::ends_with(_path, it->first)) {
          if (access(_path.c_str(), X_OK) == -1) {
            if (errno == ENOENT)
              continue;
            else if (errno == EACCES)
              _status = 403;
            else
              throw ftpp::OSError(errno, "access");
          } else
            _cgiManager = new CgiManager(*this, it->second);
          break;
        }
      }
    } catch (std::exception &) {
      _status = 500;
    }
  }
}

DefaultTask::~DefaultTask() {
  delete _cgiManager;
}

void DefaultTask::onData(std::vector<char> const &data) {
  if (_cgiManager)
    _cgiManager->onData(data);
}

void DefaultTask::onEof() {
  if (_status != -1)
    cycle.sendErrorPage(_status);
  else if (_cgiManager)
    _cgiManager->onEof();
  else
    onEofDefault();
}

void DefaultTask::onEofDefault() {
  bool isAutoindex = false;
  try {
    struct stat st;
    if (stat(_path.c_str(), &st) == -1) {
      if (errno == ENOENT)
        throw HttpException(404);
      else if (errno == EACCES)
        throw HttpException(403);
      else
        throw HttpException(500);
    }
    if (S_ISDIR(st.st_mode)) {
      bool found = false;
      LocationDefault::Indexes const &indexes = _location.getIndex();
      for (LocationDefault::Indexes::const_iterator it = indexes.begin();
           it != indexes.end(); ++it) {
        std::string index_path = _path + "/" + *it;
        if (!ftpp::starts_with(index_path, _location.getRoot()))
          continue;
        if (stat(index_path.c_str(), &st) == -1) {
          if (errno == ENOENT || errno == EACCES)
            continue;
          else
            throw HttpException(500);
        }
        if (S_ISREG(st.st_mode)) {
          found = true;
          index_path.swap(_path);
          break;
        }
      }
      if (!found) {
        if (_location.getAutoindex())
          isAutoindex = true;
        else
          throw HttpException(404);
      }
    } else if (!S_ISREG(st.st_mode))
      throw HttpException(403);
  } catch (HttpException &e) {
    cycle.sendErrorPage(e.getStatus());
    return;
  } catch (...) {
    cycle.sendErrorPage(500);
    return;
  }
  if (isAutoindex)
    sendAutoindex();
  else
    sendFile();
}

void DefaultTask::sendAutoindex() {
  Response::Headers headers;
  if (!ftpp::ends_with(cycle.getRequest().path, std::string("/"))) {
    headers["location"].push_back(cycle.getRequest().path + "/");
    cycle.send(301, headers);
    cycle.send(NULL, 0, false);
    return;
  }
  std::ostringstream oss;
  try {
    std::string path = ftpp::htmlEscape(cycle.getRequest().path);
    oss << "<html>";
    oss << "<head><title>Index of " << path << "</title></head>";
    oss << "<body>";
    oss << "<h1>Index of " << path << "</h1>";
    oss << "<hr><pre>";
    oss << "<a href=\"../ \">../</a>\n";
    DIR *dir = opendir(_path.c_str());
    try {
      if (dir == NULL) {
        if (errno == ENOENT)
          throw HttpException(404);
        else if (errno == EACCES)
          throw HttpException(403);
        else
          throw HttpException(500);
      }
      struct dirent *entry;
      while ((entry = readdir(dir))) {
        if (entry->d_name[0] == '.')
          continue;
        struct stat st;
        if (stat((_path + "/" + entry->d_name).c_str(), &st) == -1)
          throw HttpException(500);
        std::string name = entry->d_name;
        if (S_ISDIR(st.st_mode))
          name += "/";
        oss << "<a href=\"" << ftpp::htmlEscape(name) << "\">";
        if (name.size() < 50)
          oss << ftpp::htmlEscape(name) << "</a>"
              << std::string(50 - name.size(), ' ');
        else
          oss << ftpp::htmlEscape(name.substr(0, 47)) << "..></a>";
        char timestamp[18];
        strftime(timestamp, sizeof(timestamp), "%d-%b-%Y %H:%M",
                 localtime(&st.st_mtime));
        oss << " " << timestamp;
        if (S_ISDIR(st.st_mode))
          oss << std::setw(20) << "-";
        else
          oss << std::setw(20) << st.st_size;
        oss << "\n";
      }
    } catch (...) {
      closedir(dir);
      throw;
    }
    closedir(dir);
    oss << "</pre><hr>";
    oss << "</body>";
    oss << "</html>";
  } catch (HttpException &e) {
    cycle.sendErrorPage(e.getStatus());
    return;
  } catch (...) {
    cycle.sendErrorPage(500);
    return;
  }
  std::string const &body = oss.str();
  headers["content-length"].push_back(ftpp::to_string(body.size()));
  cycle.send(200, headers);
  cycle.send(body.c_str(), body.size(), false);
}

void DefaultTask::sendFile() {
  int fd = open(_path.c_str(), O_RDONLY);
  if (fd != -1) {
    try {
      Response::Headers headers;
      cycle.send(200, headers);
      for (;;) {
        char buf[4096];
        ssize_t n = read(fd, buf, sizeof(buf));
        if (n == -1)
          throw std::runtime_error("read error");
        if (n == 0)
          break;
        cycle.send(buf, n, true);
      }
      cycle.send(NULL, 0, false);
    } catch (...) {
      close(fd);
      throw;
    }
    close(fd);
  } else if (errno == ENOENT)
    cycle.sendErrorPage(404);
  else if (errno == EACCES)
    cycle.sendErrorPage(403);
  else
    cycle.sendErrorPage(500);
}

DefaultTask::CgiManager::CgiManager(DefaultTask &task,
                                    LocationDefault::Cgi const &cgi)
    : _task(task), _cgi(cgi), _process(NULL), _readPipe(NULL),
      _writePipe(NULL) {
}

DefaultTask::CgiManager::~CgiManager() {
  delete _process;
  delete _readPipe;
}

void DefaultTask::CgiManager::onData(std::vector<char> const &data) {
  _buffer.insert(_buffer.end(), data.begin(), data.end());
}

void DefaultTask::CgiManager::onEof() {
  int pipefd[4] = {-1, -1, -1, -1};
  try {
    if (pipe(&pipefd[0]) == -1 || pipe(&pipefd[2]) == -1)
      throw ftpp::OSError(errno, "pipe");
    for (int i = 0; i < 4; ++i)
      setCloexec(pipefd[i]);
    _process = new Process(_task.cycle.getLoop(), *this, pipefd[0], pipefd[3]);
    close(pipefd[0]), close(pipefd[3]);
    pipefd[0] = pipefd[3] = -1;
    _readPipe = new ReadPipe(*this, pipefd[2]);
    pipefd[2] = -1;
    _writePipe = new WritePipe(*this, pipefd[1]);
    pipefd[1] = -1;
  } catch (...) {
    delete _readPipe;
    _readPipe = NULL;
    delete _writePipe;
    _writePipe = NULL;
    delete _process;
    _process = NULL;
    for (int i = 0; i < 4; ++i) {
      if (pipefd[i] != -1)
        close(pipefd[i]);
    }
    throw;
  }
}

DefaultTask::CgiManager::Process::Process(ftev::EventLoop &loop,
                                          CgiManager &manager, int inputFd,
                                          int outputFd)
    : ProcessWatcher(loop), _manager(manager) {
  ftpp::Subprocess::options opts;
  opts.path = _manager._cgi.bin.c_str();
  char const *argv[3];
  argv[0] = _manager._cgi.bin.c_str();
  argv[1] = _manager._task._path.c_str();
  argv[2] = NULL;
  opts.argv = argv;
  std::vector<char const *> envp;
  for (char **i = environ; *i; ++i)
    envp.push_back(*i);
  envp.push_back(NULL);
  opts.envp = envp.data();
  opts.cwd = NULL;
  ftpp::Subprocess::options::pipe_pair pipePair[2];
  pipePair[0].dst = STDIN_FILENO;
  pipePair[0].src = inputFd;
  pipePair[1].dst = STDOUT_FILENO;
  pipePair[1].src = outputFd;
  opts.pipes = pipePair;
  opts.npipes = sizeof(pipePair) / sizeof(pipePair[0]);
  ftpp::Subprocess subprocess(opts);
  start(subprocess.getPid());
  subprocess.detach();
}

DefaultTask::CgiManager::Process::~Process() {
  if (getIsActive())
    kill(SIGINT), detach();
}

void DefaultTask::CgiManager::Process::onExited(int) {
}

void DefaultTask::CgiManager::Process::onSignaled(int signum) {
  ftpp::logger(ftpp::Logger::ERROR,
               ftpp::Format("CgiProcess signaled: {}") % strsignal(signum));
  onExited(signum | 128);
}

DefaultTask::CgiManager::ReadPipe::ReadPipe(CgiManager &manager, int fd)
    : _manager(manager), _transport(NULL) {
  _transport =
      new ftev::ReadPipeTransport(manager._task.cycle.getLoop(), *this, fd);
  _manager._task.cycle.send(200, Response::Headers());
}

DefaultTask::CgiManager::ReadPipe::~ReadPipe() {
  delete _transport;
}

void DefaultTask::CgiManager::ReadPipe::onData(std::vector<char> const &data) {
  try {
    _manager._task.cycle.send(data.data(), data.size(), true);
  } catch (std::exception &e) {
    ftpp::logger(ftpp::Logger::ERROR,
                 ftpp::Format("CgiProcess CgiReadPipe onData: {}") % e.what());
    _manager._task.cycle.abort();
  }
}

void DefaultTask::CgiManager::ReadPipe::onEof() {
  try {
    _manager._task.cycle.send(NULL, 0, false);
  } catch (std::exception &e) {
    ftpp::logger(ftpp::Logger::ERROR,
                 ftpp::Format("CgiProcess CgiReadPipe onEof: {}") % e.what());
    _manager._task.cycle.abort();
  }
}

DefaultTask::CgiManager::WritePipe::WritePipe(CgiManager &manager, int fd)
    : _manager(manager), _transport(NULL) {
  _transport =
      new ftev::WritePipeTransport(manager._task.cycle.getLoop(), *this, fd);
  _transport->write(_manager._buffer.data(), _manager._buffer.size());
  _manager._buffer.clear();
}

DefaultTask::CgiManager::WritePipe::~WritePipe() {
  delete _transport;
}

void DefaultTask::CgiManager::WritePipe::onDrain() {
}
