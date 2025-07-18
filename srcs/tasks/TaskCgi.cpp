/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskCgi.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:46:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/18 13:34:52 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/TaskCgi.hpp"
#include "constants.hpp"
#include "locations/LocationCgi.hpp"

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/fcntl/fcntl.hpp>
#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/pathlib/pathlib.hpp>
#include <ftpp/string/string.hpp>
#include <ftpp/subprocess/Subprocess.hpp>

#include <cassert>
#include <cstring>
#include <unistd.h>

TaskCgi::TaskCgi(WebservApp::Context const &ctx)
    : TaskStatic(ctx), _cgiManager(NULL) {
}

TaskCgi::~TaskCgi() {
  delete _cgiManager;
}

void TaskCgi::execute() {
  defaultResponse();
}

void TaskCgi::caseAutoindex(std::string const &path) {
  if (ctx.cycle.getRequest().method != "GET")
    ctx.cycle.sendErrorPage(405);
  else
    sendAutoindex(path);
}

void TaskCgi::caseFile(std::string const &path, std::string const &pathInfo) {
  LocationCgi const &location = static_cast<LocationCgi const &>(ctx.location);
  LocationCgi::Cgis const &cgis = location.getCgis();
  std::string ext;
  ftpp::getSuffix(path).swap(ext);
  LocationCgi::Cgis::const_iterator find = cgis.find(ext);
  if (find != cgis.end()) {
    try {
      _cgiManager = new CgiManager(ctx, find->second, path, pathInfo);
    } catch (...) {
      ctx.cycle.sendErrorPage(500);
    }
  } else {
    if (ctx.cycle.getRequest().method != "GET")
      ctx.cycle.sendErrorPage(405);
    else
      TaskStatic::caseFile(path, pathInfo);
  }
}

TaskCgi::CgiManager::CgiManager(WebservApp::Context const &ctx,
                                LocationCgi::Cgi const &cgi,
                                std::string const &path,
                                std::string const &pathInfo)
    : _ctx(ctx), _cgi(cgi), _path(path), _pathInfo(pathInfo), _process(NULL),
      _readPipe(NULL), _writePipe(NULL), _state(HEADER) {
  int pipefd[4] = {-1, -1, -1, -1};
  try {
    if (pipe(&pipefd[0]) == -1 || pipe(&pipefd[2]) == -1)
      throw ftpp::OSError(errno, "pipe");
    for (int i = 0; i < 4; ++i)
      ftpp::setcloexec(pipefd[i], true);
    _process = new Process(_ctx.cycle.getLoop(), *this, pipefd[0], pipefd[3]);
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

TaskCgi::CgiManager::~CgiManager() {
  delete _process;
  delete _writePipe;
  delete _readPipe;
}

TaskCgi::CgiManager::Process::Process(ftev::EventLoop &loop,
                                      CgiManager &manager, int inputFd,
                                      int outputFd)
    : ProcessWatcher(loop), _manager(manager) {
  Request const &request = _manager._ctx.cycle.getRequest();
  ftpp::Subprocess::options opts;
  opts.path = _manager._cgi.bin.c_str();
  char const *argv[3];
  argv[0] = _manager._cgi.bin.c_str();
  argv[1] = _manager._path.c_str();
  argv[2] = NULL;
  opts.argv = argv;
  std::vector<char const *> envp;
  envp.push_back("GATEWAY_INTERFACE=CGI/1.1");
  envp.push_back("SERVER_PROTOCOL=HTTP/1.1");
  envp.push_back("SERVER_SOFTWARE=Weberv");
  std::vector<std::string> envpStrs;
  envpStrs.push_back("PATH_INFO=" + (_manager._pathInfo.empty() ? "/" : _manager._pathInfo));
  envpStrs.push_back("REQUEST_METHOD=" + request.method);
  envpStrs.push_back("REQUEST_PATH=" + request.uri.getPath());
  if (!request.uri.getQuery().empty())
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

TaskCgi::CgiManager::Process::~Process() {
  if (getIsActive())
    kill(SIGINT), detach();
}

void TaskCgi::CgiManager::Process::onExited(int) {
  if (_manager._state == HEADER)
    _manager._ctx.cycle.sendErrorPage(500);
  else
    _manager._ctx.cycle.send(NULL, 0, false);
}

void TaskCgi::CgiManager::Process::onSignaled(int signum) {
  ftpp::logger(ftpp::Logger::ERROR,
               ftpp::Format("CgiProcess signaled: {}") % strsignal(signum));
  onExited(signum | 128);
}

TaskCgi::CgiManager::ReadPipe::ReadPipe(CgiManager &manager, int fd)
    : _manager(manager), _transport(NULL), _bufferClosed(false),
      _pos(0) {
  _transport =
      new ftev::ReadPipeTransport(manager._ctx.cycle.getLoop(), *this, fd);
}

TaskCgi::CgiManager::ReadPipe::~ReadPipe() {
  delete _transport;
}

void TaskCgi::CgiManager::ReadPipe::onData(std::vector<char> const &data) {
  try {
    _buffer.insert(_buffer.end(), data.begin(), data.end());
  } catch (std::exception &e) {
    ftpp::logger(ftpp::Logger::ERROR,
                 ftpp::Format("DefaultTask::CgiManager: {}") % e.what());
    _manager._ctx.cycle.abort();
    return;
  }
  _process();
}

void TaskCgi::CgiManager::ReadPipe::onEof() {
  _bufferClosed = true;
  _process();
}

void TaskCgi::CgiManager::ReadPipe::_process() {
  try {
    for (bool flag = true; flag;) {
      if (_manager._state == HEADER) {
        flag = _parseHeader();
        if (!flag && _bufferClosed)
          _manager._ctx.cycle.sendErrorPage(500);
      } else if (_manager._state == BODY) {
        std::vector<char> tmp(_buffer.begin(), _buffer.end());
        _buffer.clear();
        _manager._ctx.cycle.send(tmp.data(), tmp.size(), true);
        flag = false;
      }
    }
  } catch (std::exception &e) {
    ftpp::logger(ftpp::Logger::ERROR,
                 ftpp::Format("DefaultTask::CgiManager: {}") % e.what());
    _manager._ctx.cycle.abort();
  }
}

bool TaskCgi::CgiManager::ReadPipe::_parseHeader() {
  assert(_manager._state == HEADER);
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
    _manager._ctx.cycle.sendErrorPage(500);
    return false;
  }
  _manager._ctx.cycle.send(status, headers);
  _buffer.erase(_buffer.begin(), match + DOUBLE_CRLF.size());
  _manager._state = BODY;
  return true;
}

TaskCgi::CgiManager::WritePipe::WritePipe(CgiManager &manager, int fd)
    : _manager(manager), _transport(NULL) {
  _transport =
      new ftev::WritePipeTransport(manager._ctx.cycle.getLoop(), *this, fd);
  _transport->write(_manager._ctx.data.data(), _manager._ctx.data.size());
  _transport->drain();
}

TaskCgi::CgiManager::WritePipe::~WritePipe() {
  delete _transport;
}

void TaskCgi::CgiManager::WritePipe::onDrain() {
  _transport->close();
}
