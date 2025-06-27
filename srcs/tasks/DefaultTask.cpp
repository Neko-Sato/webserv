/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultTask.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:46:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/06/28 04:22:47 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/DefaultTask.hpp"
#include "Cycle.hpp"
#include "HttpException.hpp"
#include "constants.hpp"
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

#include <cassert>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

static inline std::string _getSuffix(const std::string &path) {
  std::string::size_type slash_pos = path.find_last_of('/');
  std::string filename =
      slash_pos == std::string::npos ? path : path.substr(slash_pos + 1);
  std::string::size_type dot_pos = filename.find_last_of('.');
  if (dot_pos == std::string::npos)
    return "";
  return filename.substr(dot_pos);
}

DefaultTask::DefaultTask(Connection::Cycle &cycle,
                         LocationDefault const &location)
    : Task(cycle), _location(location), _cgiManager(NULL), _status(-1) {
  Request const &request = cycle.getRequest();
  ftpp::normpath(_location.getRoot() + request.uri.getPath()).swap(_path);
  if (!ftpp::starts_with(_path, _location.getRoot())) {
    _status = 403;
    return;
  }
  try {
    std::string _pathSuffix = _getSuffix(_path);
    if (!_pathSuffix.empty()) {
      LocationDefault::Cgis const &cgis = _location.getCgis();
      LocationDefault::Cgis::const_iterator it = cgis.find(_pathSuffix);
      if (it != cgis.end()) {
        if (access(_path.c_str(), F_OK) == -1) {
          if (errno == ENOENT)
            _status = 404;
          else if (errno == EACCES)
            _status = 403;
          else
            throw ftpp::OSError(errno, "access");
        } else
          _cgiManager = new CgiManager(*this, it->second);
      }
    }
    if (_status == -1 && !_cgiManager && request.method != "GET")
      _status = 405;
  } catch (std::exception &) {
    _status = 500;
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
      if (errno == ENOENT || errno == ENOTDIR)
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
          if (errno == ENOENT || errno == EACCES || errno == ENOTDIR)
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
  Headers headers;
  if (!ftpp::ends_with(cycle.getRequest().uri.getPath(), std::string("/"))) {
    headers["location"].push_back(cycle.getRequest().uri.getPath() + "/");
    cycle.send(301, headers);
    cycle.send(NULL, 0, false);
    return;
  }
  std::ostringstream oss;
  try {
    std::string path = ftpp::htmlEscape(cycle.getRequest().uri.getPath());
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
      Headers headers;
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
  delete _writePipe;
  delete _readPipe;
}

void DefaultTask::CgiManager::onData(std::vector<char> const &data) {
  /*
  I really wanted to make it streamable, but that is more than necessary and not
  required for CGI, so I tried to compromise with temporary files. But this
  subject does not have the requirements to use temporary files, such as mktemp
  and lseek. lseek is not available because it is done. It is garbage shit. No
  choice but to use std::vector<char> and pipe
  */
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
    _task.cycle.sendErrorPage(500);
  }
}

DefaultTask::CgiManager::Process::Process(ftev::EventLoop &loop,
                                          CgiManager &manager, int inputFd,
                                          int outputFd)
    : ProcessWatcher(loop), _manager(manager) {
  Request const &request = _manager._task.cycle.getRequest();
  ftpp::Subprocess::options opts;
  opts.path = _manager._cgi.bin.c_str();
  char const *argv[3];
  argv[0] = _manager._cgi.bin.c_str();
  argv[1] = _manager._task._path.c_str();
  argv[2] = NULL;
  opts.argv = argv;
  std::vector<char const *> envp;
  envp.push_back("GATEWAY_INTERFACE=CGI/1.1");
  envp.push_back("SERVER_PROTOCOL=HTTP/1.1");
  envp.push_back("SERVER_SOFTWARE=Weberv");
  std::vector<std::string> envpStrs;
  envpStrs.push_back("PATH_INFO=/");
  envpStrs.push_back("SCRIPT_NAME=" + request.uri.getPath());
  envpStrs.push_back("REQUEST_METHOD=" + request.method);
  envpStrs.push_back("REQUEST_PATH=" + request.uri.getPath());
  envpStrs.push_back("REQUEST_QUERY=" + request.uri.getQuery());
  {
    Headers::const_iterator it;
    if ((it = request.headers.find("content-type")) != request.headers.end())
      envpStrs.push_back("CONTENT_TYPE=" + it->second[0]);
    if ((it = request.headers.find("content-length")) != request.headers.end())
      envpStrs.push_back("CONTENT_LENGTH=" + it->second[0]);
  }
  for (Headers::const_iterator it = request.headers.begin();
       it != request.headers.end(); ++it) {
    if (it->first == "content-type" || it->first == "content-length")
      continue;
    envpStrs.push_back(
        "HTTP_" + ftpp::toupper(it->first) + "=" +
        ftpp::strjoin(it->second.begin(), it->second.end(), ","));
  }
  for (std::vector<std::string>::const_iterator it = envpStrs.begin();
       it != envpStrs.end(); ++it)
    envp.push_back(it->c_str());
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
  _manager._task.cycle.send(NULL, 0, false);
}

void DefaultTask::CgiManager::Process::onSignaled(int signum) {
  ftpp::logger(ftpp::Logger::ERROR,
               ftpp::Format("CgiProcess signaled: {}") % strsignal(signum));
  onExited(signum | 128);
}

DefaultTask::CgiManager::ReadPipe::ReadPipe(CgiManager &manager, int fd)
    : _manager(manager), _transport(NULL), _state(HEADER), _bufferClosed(false),
      _pos(0) {
  _transport =
      new ftev::ReadPipeTransport(manager._task.cycle.getLoop(), *this, fd);
}

DefaultTask::CgiManager::ReadPipe::~ReadPipe() {
  delete _transport;
}

void DefaultTask::CgiManager::ReadPipe::onData(std::vector<char> const &data) {
  try {
    _buffer.insert(_buffer.end(), data.begin(), data.end());
  } catch (std::exception &e) {
    ftpp::logger(ftpp::Logger::ERROR,
                 ftpp::Format("DefaultTask::CgiManager: {}") % e.what());
    _manager._task.cycle.abort();
    return;
  }
  _process();
}

void DefaultTask::CgiManager::ReadPipe::onEof() {
  _bufferClosed = true;
  _process();
}

void DefaultTask::CgiManager::ReadPipe::_process() {
  try {
    for (bool flag = true; flag;) {
      if (_state == HEADER) {
        flag = _parseHeader();
        if (!flag && _bufferClosed)
          _manager._task.cycle.sendErrorPage(500);
      } else if (_state == BODY) {
        std::vector<char> tmp(_buffer.begin(), _buffer.end());
        _buffer.clear();
        _manager._task.cycle.send(tmp.data(), tmp.size(), true);
        flag = false;
      }
    }
  } catch (std::exception &e) {
    ftpp::logger(ftpp::Logger::ERROR,
                 ftpp::Format("DefaultTask::CgiManager: {}") % e.what());
    _manager._task.cycle.abort();
  }
}

bool DefaultTask::CgiManager::ReadPipe::_parseHeader() {
  assert(_state == HEADER);
  if (_buffer.size() - _pos < DOUBLE_CRLF.size())
    return false;
  std::deque<char>::iterator match =
      std::search(_buffer.begin() + _pos, _buffer.end(), DOUBLE_CRLF.begin(),
                  DOUBLE_CRLF.end());
  if (match == _buffer.end()) {
    _pos = _buffer.size() - DOUBLE_CRLF.size();
    return false;
  }
  _pos = 0;
  Headers headers;
  int status = 200;
  try {
    Headers tmp;
    parseHeaders(tmp, std::string(_buffer.begin(), match + CRLF.size()));
    if (tmp.find("content-type") == tmp.end())
      throw std::runtime_error("Invalid content-type");
    for (Headers::const_iterator it = tmp.begin(); it != tmp.end(); ++it) {
      if (it->first == "status") {
        if (it->second.size() != 1)
          throw std::runtime_error("Invalid status");
        status = ftpp::stoul(it->second[0]);
        if (status < 100 || status > 599)
          throw std::runtime_error("Invalid status");
      } else if (it->first == "transfer-encoding") {
      } else
        headers[it->first] = it->second;
    }
  } catch (std::exception &e) {
    ftpp::logger(ftpp::Logger::ERROR,
                 ftpp::Format("DefaultTask::CgiManager: {}") % e.what());
    _transport->pause();
    _manager._task.cycle.sendErrorPage(500);
    return false;
  }
  _manager._task.cycle.send(status, headers);
  _buffer.erase(_buffer.begin(), match + DOUBLE_CRLF.size());
  _state = BODY;
  return true;
}

DefaultTask::CgiManager::WritePipe::WritePipe(CgiManager &manager, int fd)
    : _manager(manager), _transport(NULL) {
  _transport =
      new ftev::WritePipeTransport(manager._task.cycle.getLoop(), *this, fd);
  _transport->write(_manager._buffer.data(), _manager._buffer.size());
  _manager._buffer.clear();
  _transport->drain();
}

DefaultTask::CgiManager::WritePipe::~WritePipe() {
  delete _transport;
}

void DefaultTask::CgiManager::WritePipe::onDrain() {
  _transport->close();
}
