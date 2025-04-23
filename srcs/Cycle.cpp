/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cycle.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 23:45:55 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 03:44:58 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cycle.hpp"
#include "Connection.hpp"
#include "HttpException.hpp"
#include "constants.hpp"

#include <ftpp/string/string.hpp>
#include <ftpp/urllib/URI.hpp>

#include <cassert>

/*
見直す必要がある
リクエストヘッダーについて適当かどうか検証して適切にエラーページを返す必要がある
*/
Connection::Cycle::Cycle(Connection &connection)
    : _connection(connection), _state(RESPONSE), _serverConf(NULL), _app(NULL),
      _reader(NULL), _writer(NULL), _eof(false), _bodySize(0) {
  try {
    try {
      Request::Headers::const_iterator it;
      it = _connection._request.headers.find("host");
      _serverConf = &_connection._configs.findServer(
          _connection._address,
          it != _connection._request.headers.end() ? &it->second.back() : NULL);
      it = _connection._request.headers.find("connection");
      if (it != _connection._request.headers.end()) {
        if (it->second.size() != 1)
          throw HttpException(400);
        else if (it->second.back() == "keep-alive")
          _connection._keepAlive = true;
        else if (it->second.back() == "close")
          _connection._keepAlive = false;
        else
          throw HttpException(400);
      }
      it = _connection._request.headers.find("transfer-encoding");
      if (it != _connection._request.headers.end()) {
        if (it->second.size() != 1)
          throw HttpException(400);
        else if (it->second.back() == "chunked")
          _reader = new ChankedReader;
        else
          throw HttpException(400);
      } else {
        it = _connection._request.headers.find("content-length");
        if (it != _connection._request.headers.end()) {
          std::string const &nstr = it->second.back();
          std::size_t len;
          std::size_t n = ftpp::stoul(nstr, &len);
          if (nstr.size() != len)
            throw HttpException(400);
          else
            _reader = new ContentLengthReader(n);
        }
      }
      _app = new App(*this);
    } catch (HttpException &e) {
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
    sendErrorPage(e.getCode());
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
    _bodySize += tmp.size();
    if (_bodySize > _serverConf->getClientMaxBodySize()) {
      _app->onCancel();
      delete _app;
      _app = NULL;
      sendErrorPage(413);
    }
    if (_app && !tmp.empty())
      _app->onData(tmp);
  }
  if (!_reader || _reader->completed()) {
    ftev::StreamConnectionTransport &transport = _connection.getTransport();
    if (_app)
      _app->onEof();
    _eof = true;
    transport.pause();
    delete _reader;
    _reader = NULL;
    if (_state == DONE)
      _connection._complete->start();
  } else if (_connection._bufferClosed)
    throw std::runtime_error("interrupted body");
}

ServerConf const &Connection::Cycle::getServerConf() const {
  assert(_serverConf);
  return *_serverConf;
}

Request const &Connection::Cycle::getRequest() const {
  return _connection._request;
}

/*
見直す必要がある
ここの目的はレスポンスヘッダーが正しいかチェックしどうしても不正な場合はrunntime_errorを
訂正可能な場合は修正して送信する。
*/
void Connection::Cycle::send(int code, Response::Headers const &headers) {
  assert(_state == RESPONSE);
  std::string response;
  {
    Response tmp;
    tmp.version = "HTTP/1.1";
    tmp.status = code;
    {
      HttpStatusMap::const_iterator it = httpStatusMap.find(code);
      tmp.reason = it != httpStatusMap.end() ? it->second : "Unknown";
      for (Response::Headers::const_iterator it = headers.begin();
           it != headers.end(); ++it) {
        Response::HeaderValues &values = tmp.headers[ftpp::tolower(it->first)];
        for (Response::HeaderValues::const_iterator jt = it->second.begin();
             jt != it->second.end(); ++jt)
          values.push_back(*jt);
      }
    }
    if (_connection._keepAlive)
      tmp.headers["connection"].push_back("keep-alive");
    else
      tmp.headers["connection"].push_back("close");
    {
      Response::Headers::iterator it = tmp.headers.find("content-length");
      if (it == tmp.headers.end())
        _writer = new ChankedWriter;
      else {
        std::string const &nstr = it->second.back();
        std::size_t len;
        std::size_t n = ftpp::stoul(nstr, &len, 10);
        if (nstr.size() != len)
          throw std::runtime_error("invalid content-length");
        _writer = new ContentLengthWriter(n);
      }
    }
    composeResponse(response, tmp);
  }
  ftev::StreamConnectionTransport &transport = _connection.getTransport();
  transport.write(response.c_str(), response.size());
  _state = BODY;
}

void Connection::Cycle::send(char const *data, std::size_t size, bool more) {
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

/*
見直す必要がある
上のsend関数を用いて適当にエラーページを返す必要がある。
失敗はrunntime_errorで良い。
*/
void Connection::Cycle::sendErrorPage(int code) {
  send(code, Response::Headers());
  send("<html><head><title>Webserv</title></head><body>"
       "<h1>Webserv</h1><p>error</p></body></html>",
       0, false);
}
