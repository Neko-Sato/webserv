/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultTask.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:46:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/01 04:53:16 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/DefaultTask.hpp"
#include "Cycle.hpp"
#include "HttpException.hpp"

#include <ftpp/algorithm.hpp>
#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/html/html.hpp>
#include <ftpp/pathlib/pathlib.hpp>
#include <ftpp/string/string.hpp>
#include <ftpp/subprocess/Subprocess.hpp>

#include <cerrno>
#include <dirent.h>
#include <fcntl.h>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

DefaultTask::DefaultTask(Connection::Cycle &cycle,
                         LocationDefault const &location)
    : Task(cycle), _location(location), _cgi(NULL), _status(-1) {
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
          //   _cgi = new CgiProcess(cycle.getLoop(), *this, it->second);
          break;
        }
      }
    } catch (std::exception &) {
      _status = 500;
    }
  }
}

DefaultTask::~DefaultTask() {
  delete _cgi;
}

void DefaultTask::onData(std::vector<char> const &data) {
  //   if (_cgi)
  //     _cgi->onData(data);
  (void)data;
}

void DefaultTask::onEof() {
  if (_status != -1)
    cycle.sendErrorPage(_status);
  else if (_cgi)
    ; //_cgi->onEof();
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

void DefaultTask::onCancel() {
  if (_cgi)
    cycle.abort();
}

// DefaultTask::CgiProcess::CgiProcess(ftev::EventLoop &loop,
//                                     std::string const &bin, DefaultTask
//                                     &task)
//     : ProcessWatcher(loop), _writePipe(NULL), _readPipe(NULL) {
//   try {
//     ftpp::Subprocess::options opts;
//     opts.path = bin.c_str();
//     char const *argv[] = {bin.c_str(), task._path.c_str(), NULL};
//     opts.argv = argv;
//     std::vector<char const *> env;
//     for (char **i = environ; *environ; ++i)
//       env.push_back(*i);
//     opts.envp = env.data();
//     opts.cwd = NULL;
//     ftpp::Subprocess::options::pipe_pair pipePair[2];
//     opts.pipes = pipePair;
//     opts.npipes = sizeof(pipePair) / sizeof(pipePair[0]);
//     int fds[2][2] = {{-1, -1}, {-1, -1}};
//     try {
//       if (pipe(fds[0]) == -1 || pipe(fds[1]) == -1)
//         throw std::runtime_error("pipe error");
//       int res = fcntl(fds[0][0], F_GETFD);
//       if (res == -1 || fcntl(fds[0][0], F_SETFD, res | FD_CLOEXEC) == -1)
//         throw ftpp::OSError(errno, "fcntl");
//       res = fcntl(fds[0][0], F_GETFD);
//       if (fcntl(fds[0][1], F_SETFD, FD_CLOEXEC) == -1)
//         if (fcntl(fds[1][0], F_SETFD, FD_CLOEXEC) == -1)
//           throw ftpp::OSError(errno, "fcntl");
//       opts.pipes[0].dst = STDIN_FILENO;
//       opts.pipes[0].src = fds[0][0];
//       opts.pipes[1].dst = STDOUT_FILENO;
//       opts.pipes[1].src = fds[1][1];
//       ftpp::Subprocess subprocess(opts);
//       close(fds[0][0]);
//       fds[0][1] = -1;
//       close(fds[1][1]);
//       fds[1][0] = -1;
//       _writePipe = new CgiWritePipe(*this, fds[0][1]);
//       fds[0][1] = -1;
//       _readPipe = new CgiReadPipe(*this, fds[1][0]);
//       fds[1][0] = -1;
//       start(subprocess.getPid());
//       subprocess.detach();
//     } catch (...) {
//       if (fds[0][0] != -1)
//         close(fds[0][0]);
//       if (fds[0][1] != -1)
//         close(fds[0][1]);
//       if (fds[1][0] != -1)
//         close(fds[1][0]);
//       if (fds[1][1] != -1)
//         close(fds[1][1]);
//       throw;
//     }
//   } catch (...) {
//     delete _writePipe;
//     delete _readPipe;
//     throw;
//   }
// }

// DefaultTask::CgiProcess::~CgiProcess() {
//   if (getIsActive())
//     kill(SIGINT), detach();
//   delete _writePipe;
//   delete _readPipe;
// }

// void DefaultTask::CgiProcess::onExited(int) {
// }

// void DefaultTask::CgiProcess::onSignaled(int) {
//   _task.cycle.abort();
// }

// void DefaultTask::CgiProcess::onData(std::vector<char> const &data) {
//   ftev::WritePipeTransport &transport = _writePipe->getTransport();
//   transport.write(data.data(), data.size());
// }

// void DefaultTask::CgiProcess::onEof() {
//   ftev::WritePipeTransport &transport = _writePipe->getTransport();
//   transport.close();
// }

// DefaultTask::CgiProcess::CgiWritePipe::CgiWritePipe(CgiProcess &process, int
// fd)
//     : _process(process), _writePipe(NULL) {
//   _writePipe = new ftev::WritePipeTransport(process.loop, *this, fd);
// }

// DefaultTask::CgiProcess::CgiWritePipe::~CgiWritePipe() {
//   delete _writePipe;
// }

// ftev::WritePipeTransport &
// DefaultTask::CgiProcess::CgiWritePipe::getTransport() {
//   return *_writePipe;
// }

// void DefaultTask::CgiProcess::CgiWritePipe::onDrain() {
// }

// void DefaultTask::CgiProcess::CgiWritePipe::onExcept() {
// }

// DefaultTask::CgiProcess::CgiReadPipe::CgiReadPipe(CgiProcess &process, int
// fd)
//     : _process(process), _readPipe(NULL) {
//   _readPipe = new ftev::ReadPipeTransport(process.loop, *this, fd);
// }

// DefaultTask::CgiProcess::CgiReadPipe::~CgiReadPipe() {
//   delete _readPipe;
// }

// void DefaultTask::CgiProcess::CgiReadPipe::onData(
//     std::vector<char> const &data) {
//   /*Cgiの返り値についてガッツリ検証するStateマシンがここには必要かと思う。*/
// }

// void DefaultTask::CgiProcess::CgiReadPipe::onEof() {
//   _process._task.cycle.send(NULL, 0, false);
// }

// void DefaultTask::CgiProcess::CgiReadPipe::onExcept() {
// }
