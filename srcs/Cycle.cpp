/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cycle.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 23:45:55 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 16:22:22 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebservApp.hpp"
#include "Cycle.hpp"
#include "Connection.hpp"
#include "HttpException.hpp"
#include "constants.hpp"
#include "utility.hpp"

#include <ftpp/algorithm.hpp>
#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/string/string.hpp>
#include <ftpp/urllib/URI.hpp>

#include <cassert>
#include <fcntl.h>
#include <unistd.h>

Connection::Cycle::Cycle(Connection &connection)
    : _connection(connection), _state(RESPONSE), _serverConf(NULL), _app(NULL),
      _reader(NULL), _writer(NULL), _eof(false) {
  try {
    try {
      Headers const &headers = _connection._request.headers;
      Headers::const_iterator it;
      {
        std::string const *host = NULL;
        if ((it = headers.find("host")) != headers.end()) {
          if (it->second.size() != 1)
            throw HttpException(400);
          host = &it->second.back();
        }
        _serverConf =
            &_connection._configs.findServer(_connection._address, host);
      }
      if (_connection._request.version != "HTTP/1.1")
        throw HttpException(505);
      if (!ftpp::starts_with(_connection._request.uri.getPath(),
                             std::string("/")))
        throw HttpException(400);
      if ((it = headers.find("connection")) != headers.end()) {
        if (it->second.size() != 1)
          throw HttpException(400);
        else if (it->second.back() == "keep-alive")
          _connection._keepAlive = true;
        else if (it->second.back() == "close")
          _connection._keepAlive = false;
        else
          throw HttpException(400);
      }
      if ((it = headers.find("content-type")) != headers.end()) {
        if (it->second.size() != 1)
          throw HttpException(400);
      }
      if ((it = headers.find("transfer-encoding")) != headers.end()) {
        if (it->second.size() != 1 ||
            headers.find("content-length") != headers.end())
          throw HttpException(400);
        else if (it->second.back() == "chunked")
          _reader = new ChankedReader;
        else
          throw HttpException(501);
      } else {
        if ((it = headers.find("content-length")) != headers.end()) {
          if (it->second.size() != 1)
            throw HttpException(400);
          std::string const &nstr = it->second.back();
          std::size_t len;
          std::size_t n = ftpp::stoul(nstr, &len);
          if (nstr.size() != len)
            throw HttpException(400);
          else
            _reader = new ContentLengthReader(n);
        }
      }
      _app = new WebservApp(*this);
    } catch (HttpException &) {
      throw;
    } catch (...) {
      throw HttpException(500);
    }
  } catch (HttpException &e) {
    delete _reader;
    delete _app;
    _reader = NULL;
    _app = NULL;
    _connection._keepAlive = false;
    sendErrorPage(e.getStatus());
  }
}

Connection::Cycle::~Cycle() {
  delete _writer;
  delete _reader;
  delete _app;
}

void Connection::Cycle::bufferUpdate() {
  if (_reader) {
    std::vector<char> tmp;
    _reader->read(_connection._buffer, tmp);
    if (_app && !tmp.empty())
      _app->onData(tmp);
  }
  if (!_reader || _reader->completed()) {
    ftev::StreamConnectionTransport &transport = _connection.getTransport();
    if (_app)
      _app->onEof();
    transport.pause();
    _eof = true;
    delete _reader;
    _reader = NULL;
    if (_state == DONE)
      _connection._complete->start();
  } else if (_connection._bufferClosed)
    throw std::runtime_error("interrupted body");
}

ftev::EventLoop &Connection::Cycle::getLoop() {
  return _connection.loop;
}

ServerConf const &Connection::Cycle::getServerConf() const {
  assert(_serverConf);
  return *_serverConf;
}

Request const &Connection::Cycle::getRequest() const {
  return _connection._request;
}

static char const *_resetColorEscape = "\x1b[0m";

static char const *_statusColorEscape(int code) {
  switch (code / 100) {
  case 1:
    return "\x1b[97m";
  case 2:
    return "\x1b[32m";
  case 3:
    return "\x1b[33m";
  case 4:
    return "\x1b[31m";
  case 5:
    return "\x1b[91m";
  default:
    return "\x1b[0m";
  }
}

void Connection::Cycle::send(int code, Headers const &headers) {
  assert(_state == RESPONSE);
  std::string response;
  {
    Response tmp;
    tmp.version = "HTTP/1.1";
    tmp.status = code;
    tmp.reason = getHttpStatusReason(code);
    for (Headers::const_iterator it = headers.begin();
         it != headers.end(); ++it) {
      HeaderValues &values = tmp.headers[ftpp::tolower(it->first)];
      for (HeaderValues::const_iterator jt = it->second.begin();
           jt != it->second.end(); ++jt)
        values.push_back(*jt);
    }
    Headers::iterator it;
    if ((it = tmp.headers.find("connection")) != tmp.headers.end()) {
      if (it->second.size() != 1)
        throw std::runtime_error("invalid connection");
      else if (it->second.back() == "close")
        _connection._keepAlive = false;
      else if (it->second.back() != "keep-alive")
        throw std::runtime_error("invalid connection");
    } else {
      if (_connection._keepAlive)
        tmp.headers["connection"].push_back("keep-alive");
      else
        tmp.headers["connection"].push_back("close");
    }
    if ((it = tmp.headers.find("content-length")) != tmp.headers.end()) {
      if (it->second.size() != 1 ||
          headers.find("transfer-encoding") != headers.end())
        throw std::runtime_error("invalid content-length");
      std::string const &nstr = it->second.back();
      std::size_t len;
      std::size_t n = ftpp::stoul(nstr, &len, 10);
      if (nstr.size() != len)
        throw std::runtime_error("invalid content-length");
      _writer = new ContentLengthWriter(n);
    } else {
      if ((it = tmp.headers.find("transfer-encoding")) != tmp.headers.end()) {
        if (it->second.size() != 1 || it->second.back() != "chunked")
          throw std::runtime_error("invalid transfer-encoding");
      } else
        tmp.headers["transfer-encoding"].push_back("chunked");
      _writer = new ChankedWriter;
    }
    composeResponse(response, tmp);
#if defined(NOCOLOR)
    ftpp::logger(ftpp::Logger::INFO,
                 ftpp::Format("{} {} -> {} {}") % _connection._request.method %
                     _connection._request.path % tmp.status % tmp.reason);
#else
    ftpp::logger(ftpp::Logger::INFO,
                 ftpp::Format("{} {} -> {}{} {}{}") %
                     _connection._request.method % _connection._request.uri %
                     _statusColorEscape(tmp.status) % tmp.status % tmp.reason %
                     _resetColorEscape);
#endif
  }
  ftev::StreamConnectionTransport &transport = _connection.getTransport();
  transport.write(response.c_str(), response.size());
  _state = BODY;
}

void Connection::Cycle::send(char const *data, std::size_t size, bool more) {
  assert(_writer);
  assert(_state == BODY);
  _writer->write(_connection.getTransport(), data, size, more);
  if (!more) {
    delete _writer;
    _writer = NULL;
    _state = DONE;
    if (_eof && (!_reader || _reader->completed()))
      _connection._complete->start();
  }
}

void Connection::Cycle::sendErrorPage(int code) {
  ServerConf::ErrorPages const &errorPages = _serverConf->getErrorPages();
  ServerConf::ErrorPages::const_iterator it = errorPages.find(code);
  if (it != errorPages.end()) {
    int fd = open(it->second.c_str(), O_RDONLY);
    if (fd != -1) {
      try {
        Headers headers;
        send(code, headers);
        for (;;) {
          char buf[4096];
          ssize_t n = read(fd, buf, sizeof(buf));
          if (n == -1)
            throw std::runtime_error("read error");
          if (n == 0)
            break;
          send(buf, n, true);
        }
        send(NULL, 0, false);
      } catch (...) {
        close(fd);
        throw;
      }
      close(fd);
      return;
    }
  }
  Headers headers;
  std::ostringstream oss;
  std::string const &reason = getHttpStatusReason(code);
  oss << "<!DOCTYPE html>";
  oss << "<html>";
  oss << "<head>";
  oss << "<title>" << reason << "</title>";
  oss << "</head>";
  oss << "<body>";
  oss << "<center><h1>" << reason << "</h1></center>";
  oss << "<hr><center>webserv</center>";
  oss << "</body>";
  oss << "</html>";
  std::string const &body = oss.str();
  headers["content-type"].push_back("text/html");
  headers["content-length"].push_back(ftpp::to_string(body.size()));
  send(code, headers);
  send(body.c_str(), body.size(), false);
}

void Connection::Cycle::abort() {
  ftev::StreamConnectionTransport &transport = _connection.getTransport();
  transport.pause();
  _connection.release();
}
