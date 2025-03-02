/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:41:18 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/02 10:17:49 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "constants.hpp"

#include <ft_iterator.hpp>
#include <ft_string.hpp>
#include <urllib/URI.hpp>
#include <urllib/urlquote.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

Connection::Connection(ftev::EventLoop &loop, ftpp::Socket &socket,
                       Server const &server)
    : ftev::BaseTCPConnection(loop, socket), _server(server), _state(REQUEST),
      _receiveRequestPosition(0) {
}

Connection::~Connection() {
}

void Connection::_process() {
  for (bool flag = true; flag;) {
    switch (_state) {
    case REQUEST:
      flag = _receiveRequest();
      break;
    case BODY:
      flag = _receiveBody();
      break;
    case RESPONSE:
      flag = false;
      break;
    case DONE:
      if (is_active())
        stop();
      delete_later();
      flag = false;
      break;
    }
  }
}

void Connection::on_data(std::vector<char> const &data) {
  _buffer.insert(_buffer.end(), data.begin(), data.end());
  _process();
}

void Connection::on_eof() {
  // 送るもん送ったら切断する
  // 変なsttｚ7
}

void Connection::on_drain() {
  switch (_state) {
  case BODY:
    // ボディの受信完了待ち、受信内容は破棄される。
    break;
  case RESPONSE:
    // ここに来たということはresponseを返したという事
    _state = DONE;
    break;
  default:
    // それ以外はありえない
    assert(false);
  }
  _process();
}

void Connection::on_except() {
  if (is_active())
    stop();
  delete_later();
}

void Connection::on_release() {
  delete this;
}

bool Connection::_receiveRequest() {
  {
    if (_buffer.size() < DOUBLE_CRLF.size())
      return false;
    std::deque<char>::iterator match =
        std::search(_buffer.begin() + _receiveRequestPosition, _buffer.end(),
                    DOUBLE_CRLF.begin(), DOUBLE_CRLF.end());
    if (match == _buffer.end()) {
      _receiveRequestPosition = _buffer.size() - DOUBLE_CRLF.size();
      return false;
    }
    _receiveRequestPosition = 0;
    std::string tmp(_buffer.begin(), match + CRLF.size());
    _buffer.erase(_buffer.begin(), match + DOUBLE_CRLF.size());
    parseRequest(tmp).swap(_request);
  }
  _state = RESPONSE;
  write("HTTP/1.1 200 OK\r\n", 17);
  write("Content-Type: text/plain\r\n", 26);
  write("Content-Length: 5\r\n", 19);
  write("\r\n", 2);
  write("Hello", 5);
  drain();
  return true;
}

bool Connection::_receiveBody() {
  return false;
}
