/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:41:18 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/12 00:29:00 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "constants.hpp"

#include <ftpp/format/Format.hpp>
#include <ftpp/iterator.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/macros.hpp>
#include <ftpp/string/string.hpp>
#include <ftpp/urllib/URI.hpp>
#include <ftpp/urllib/urlquote.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

Connection::Timeout::Timeout(ftev::EventLoop &loop, Connection &connection)
    : TimerWatcher(loop), _connection(connection) {
}

Connection::Timeout::~Timeout() {
  if (is_active())
    cancel();
}

void Connection::Timeout::on_timeout() {
  ftpp::logger(ftpp::Logger::INFO, "Connection timeout");
  ftev::StreamConnectionTransport &transport = _connection.get_transport();
  transport.close();
  _connection.release();
}

Connection::Connection(ftev::EventLoop &loop, ftpp::Socket &socket,
                       Address const &address, Configs const &configs)
    : TCPConnection(loop, socket), DeferredDelete(loop), _address(address),
      _configs(configs), _state(REQUEST), _bufferClosed(false),
      _receiveRequestPosition(0), _cycle(NULL), _timeout(NULL) {
  ftpp::logger(ftpp::Logger::INFO, "Connection connected");
  _timeout = new Timeout(loop, *this);
  _timeout->start(10000);
}

Connection::~Connection() {
  ftpp::logger(ftpp::Logger::INFO, "Connection close");
  delete _cycle;
  delete _timeout;
}

void Connection::on_data(std::vector<char> const &data) {
  try {
    _buffer.insert(_buffer.end(), data.begin(), data.end());
  } catch (std::exception &e) {
    ftpp::logger(ftpp::Logger::ERROR,
                 ftpp::Format("Connection error: {}") % e.what());
    ftev::StreamConnectionTransport &transport = get_transport();
    transport.close();
    release();
    return;
  }
  _process();
}

void Connection::on_eof() {
  _bufferClosed = true;
  _process();
}

void Connection::on_drain() {
  assert(_state == RESPONSE);
  _state = DONE;
  _process();
}

void Connection::on_except() {
  ftpp::logger(ftpp::Logger::ERROR, "Connection except");
  ftev::StreamConnectionTransport &transport = get_transport();
  transport.close();
  release();
}

void Connection::on_release() {
  delete this;
}

void Connection::_process() {
  try {
    for (bool flag = true; flag;) {
      switch (_state) {
      case REQUEST:
        flag = _process_request();
        if (!flag && _bufferClosed) {
          ftev::StreamConnectionTransport &transport = get_transport();
          transport.close();
          release();
        }
        break;
      case RESPONSE:
        _cycle->bufferUpdate(_buffer, _bufferClosed);
        flag = false;
        break;
      case DONE:
        _timeout->cancel();
        bool keep = _cycle->getKeepAlive();
        delete _cycle;
        _cycle = NULL;
        ftev::StreamConnectionTransport &transport = get_transport();
        ftpp::logger(ftpp::Logger::INFO,
                     ftpp::Format("Connection keep alive: {}") %
                         (keep ? "true" : "false"));
        if (keep) {
          _state = REQUEST;
          transport.resume();
          _timeout->start(10000);
        } else {
          transport.close();
          release();
          flag = false;
        }
        break;
      };
    }
  } catch (std::exception &e) {
    ftpp::logger(ftpp::Logger::ERROR,
                 ftpp::Format("Connection error: {}") % e.what());
    ftev::StreamConnectionTransport &transport = get_transport();
    transport.close();
    release();
  }
}

bool Connection::_process_request() {
  assert(_state == REQUEST);
  if (_buffer.size() - _receiveRequestPosition < DOUBLE_CRLF.size())
    return false;
  std::deque<char>::iterator match =
      std::search(_buffer.begin() + _receiveRequestPosition, _buffer.end(),
                  DOUBLE_CRLF.begin(), DOUBLE_CRLF.end());
  if (match == _buffer.end()) {
    _receiveRequestPosition = _buffer.size() - DOUBLE_CRLF.size();
    return false;
  }
  _receiveRequestPosition = 0;
  parseRequest(std::string(_buffer.begin(), match + CRLF.size()))
      .swap(_request);
  _buffer.erase(_buffer.begin(), match + DOUBLE_CRLF.size());
  _state = RESPONSE;
  _cycle = new Cycle(get_transport(), _configs, _request, _address);
  return true;
}
