/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:41:18 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/15 23:41:57 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "constants.hpp"

#include <ft_iterator.hpp>
#include <ft_string.hpp>
#include <logger/Logger.hpp>
#include <Format.hpp>
#include <urllib/URI.hpp>
#include <urllib/urlquote.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

Connection::Connection(ftev::EventLoop &loop, ftpp::Socket &socket,
                       Server const &server)
    : BaseTCPConnection(loop, socket), _server(server), _state(REQUEST),
      _bufferClosed(false), _receiveRequestPosition(0)
// , _cycle(NULL)
{
  ftpp::logger.log(ftpp::Logger::INFO, "Connection connected");
}

Connection::~Connection() {
  //   delete _cycle;
  ftpp::logger.log(ftpp::Logger::INFO, "Connection close");
}

void Connection::_process() {
  for (bool flag = true; flag;) {
    switch (_state) {
    case REQUEST:
      flag = _receiveRequest();
      break;
    case RESPONSE:
      flag = _receiveBody();
      break;
    case DONE:
      flag = _completed();
      break;
    }
  }
}

void Connection::on_data(std::vector<char> const &data) {
  ftpp::logger.log(ftpp::Logger::INFO, "Connection on_data");
  try {
    _buffer.insert(_buffer.end(), data.begin(), data.end());
    _process();
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    if (is_active())
      stop();
    delete_later();
  }
}

void Connection::on_eof() {
  ftpp::logger.log(ftpp::Logger::INFO, "Connection on_eof");
  try {
    _bufferClosed = true;
    _process();
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    if (is_active())
      stop();
    delete_later();
  }
}

void Connection::on_drain() {
  try {
    assert(_state == RESPONSE);
    _state = DONE;
    _process();
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    if (is_active())
      stop();
    delete_later();
  }
}

void Connection::on_except() {
  ftpp::logger.log(ftpp::Logger::INFO, "Connection on_except");
  if (is_active())
    stop();
  delete_later();
}

void Connection::on_release() {
  delete this;
}

bool Connection::_receiveRequest() {
  assert(_state == REQUEST);
  //   assert(!_cycle);
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
  parseRequest(std::string(_buffer.begin(), match + CRLF.size()))
      .swap(_request);
  _buffer.erase(_buffer.begin(), match + DOUBLE_CRLF.size());
  _state = RESPONSE;
  // httpのversionを確認1.1それ以外は505
  // Connection: keep-aliveを確認
  // transfer-encodingの確認
  //   chunkedの場合はchunkedを確認
  //   chunkedでない場合は400
  // content-lengthの確認
  // Readerを作成 constructor内で任意のerror　
  // Taskを作成　constructor内で任意のerror
  // cathcでstatsuを受け取りerror_pageを返すTaskを作成。Connectionはcloseに
  return true;
}

bool Connection::_receiveBody() {
  assert(_state == RESPONSE);
  //   assert(_cycle);
  // Readerにデータを渡しreaderは
  // そこから必要分を取り出す。
  // eofを迎えているのに足りないときは即座にConnectionをcloseする
  return false;
}

bool Connection::_completed() {
  //   assert(_cycle);
  // タスクの処理が完了したらここに来る。
  // responseは正しく返されている。
  // keepaliveがtrueならstateをREQUESTに戻しタイムアウトの設定。
  // keepaliveがfalseならこのconnectionをcloseする。
  return false;
}
