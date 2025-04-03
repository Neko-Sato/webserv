/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:41:18 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/03 16:32:58 by hshimizu         ###   ########.fr       */
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

Connection::Connection(ftev::EventLoop &loop, ftpp::Socket &socket,
                       Address const &address, Configs const &configs)
    : TCPConnection(loop, socket), DeferredDelete(loop), _address(address),
      _configs(configs), _state(REQUEST), _bufferClosed(false),
      _receiveRequestPosition(0), _task(NULL) {
  ftpp::logger(ftpp::Logger::INFO, "Connection connected");
  UNUSED(_configs);
}

Connection::~Connection() {
  ftpp::logger(ftpp::Logger::INFO, "Connection close");
  delete _task;
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
        if (!flag && _bufferClosed)
          throw std::runtime_error("closed");
        break;
      case RESPONSE:
        (*_task)(_buffer);
        flag = false;
        break;
      case DONE:
        delete _task;
        _task = NULL;
        _state = REQUEST;
        ftev::StreamConnectionTransport &transport = get_transport();
        transport.resume();
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
  _task = new Task(get_transport(), _configs, _request, _address);
  return true;
}
