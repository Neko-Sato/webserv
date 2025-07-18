/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:41:18 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/18 14:11:39 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "Cycle.hpp"
#include "constants.hpp"

#include <ftpp/format/Format.hpp>
#include <ftpp/iterator.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/string/string.hpp>
#include <ftpp/urllib/URI.hpp>

#include <algorithm>
#include <cassert>

Connection::App::App(Connection::Cycle &cycle) : cycle(cycle) {
}

Connection::App::~App() {
}

time_t const Connection::requestTimeout = 60000;

Connection::Timeout::Timeout(ftev::EventLoop &loop, Connection &connection)
    : TimerWatcher(loop), _connection(connection) {
}

Connection::Timeout::~Timeout() {
  if (getIsActive())
    cancel();
}

void Connection::Timeout::onTimeout() {
  ftpp::logger(ftpp::Logger::INFO, "Connection timeout");
  ftev::StreamConnectionTransport &transport = _connection.getTransport();
  transport.close();
  _connection.release();
}

Connection::Complete::Complete(ftev::EventLoop &loop, Connection &connection)
    : DeferWatcher(loop), _connection(connection) {
}

Connection::Complete::~Complete() {
  if (getIsActive())
    cancel();
}

void Connection::Complete::onEvent() {
  _connection._state = DONE;
  _connection._process();
}

Connection::Connection(ftev::EventLoop &loop, ftpp::Socket &socket,
                       Address const &address, Configs const &configs)
    : TCPConnection(loop, socket), Reaper(loop), _address(address),
      _configs(configs), _state(REQUEST), _bufferClosed(false),
      _receiveRequestPosition(0), _cycle(NULL), _timeout(NULL), _complete(NULL),
      _keepAlive(false) {
  ftpp::logger(ftpp::Logger::INFO, "Connection connected");
  try {
    _timeout = new Timeout(loop, *this);
    _complete = new Complete(loop, *this);
    _timeout->start(requestTimeout);
  } catch (...) {
    delete _complete;
    delete _timeout;
    throw;
  }
}

Connection::~Connection() {
  ftpp::logger(ftpp::Logger::INFO, "Connection close");
  delete _cycle;
  delete _complete;
  delete _timeout;
}

void Connection::onData(std::vector<char> const &data) {
  try {
    _buffer.insert(_buffer.end(), data.begin(), data.end());
  } catch (std::exception &e) {
    ftpp::logger(ftpp::Logger::ERROR,
                 ftpp::Format("Connection error: {}") % e.what());
    ftev::StreamConnectionTransport &transport = getTransport();
    transport.close();
    release();
    return;
  }
  _process();
}

void Connection::onEof() {
  _bufferClosed = true;
  _process();
}

void Connection::onDrain() {
  ftev::StreamConnectionTransport &transport = getTransport();
  transport.close();
  release();
}

void Connection::onError(std::exception const &exce) {
  ftpp::logger(ftpp::Logger::ERROR,
               ftpp::Format("Connection error: {}") % exce.what());
  ftev::StreamConnectionTransport &transport = getTransport();
  transport.close();
  release();
}

void Connection::onRelease() {
  delete this;
}

void Connection::_process() {
  try {
    for (bool flag = true; flag;) {
      switch (_state) {
      case REQUEST:
        flag = _processRequest();
        if (!flag && _bufferClosed) {
          ftev::StreamConnectionTransport &transport = getTransport();
          transport.drain();
          flag = false;
        }
        break;
      case RESPONSE:
        _cycle->bufferUpdate();
        flag = false;
        break;
      case DONE:
        _timeout->cancel();
        delete _cycle;
        _cycle = NULL;
        ftev::StreamConnectionTransport &transport = getTransport();
        if (_keepAlive) {
          _state = REQUEST;
          transport.resume();
          _timeout->start(requestTimeout);
        } else {
          transport.drain();
          flag = false;
        }
        break;
      };
    }
  } catch (std::exception &e) {
    ftpp::logger(ftpp::Logger::ERROR,
                 ftpp::Format("Connection error: {}") % e.what());
    ftev::StreamConnectionTransport &transport = getTransport();
    transport.close();
    release();
  }
}

bool Connection::_processRequest() {
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
  parseRequest(_request, std::string(_buffer.begin(), match + CRLF.size()));
  _buffer.erase(_buffer.begin(), match + DOUBLE_CRLF.size());
  _state = RESPONSE;
  _cycle = new Cycle(*this);
  return true;
}
