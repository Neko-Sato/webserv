/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:41:18 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 14:31:53 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "constants.hpp"

#include <ftpp/format/Format.hpp>
#include <ftpp/iterator.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/string/string.hpp>
#include <ftpp/urllib/URI.hpp>
#include <ftpp/urllib/urlquote.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

Connection::Connection(ftev::EventLoop &loop, ftpp::Socket &socket,
                       Address const &address, Configs const &configs)
    : StreamConnection(loop, socket), _address(address), _configs(configs),
      _state(REQUEST), _bufferClosed(false), _receiveRequestPosition(0) {
  ftpp::logger(ftpp::Logger::INFO, "Connection connected");
}

Connection::~Connection() {
  ftpp::logger(ftpp::Logger::INFO, "Connection close");
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
  ftpp::logger(ftpp::Logger::INFO, "Connection on_data");
  try {
    _buffer.insert(_buffer.end(), data.begin(), data.end());
    _process();
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    release();
  }
}

void Connection::on_eof() {
  ftpp::logger(ftpp::Logger::INFO, "Connection on_eof");
  try {
    _bufferClosed = true;
    _process();
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    release();
  }
}

void Connection::on_drain() {
  try {
    assert(_state == RESPONSE);
    _state = DONE;
    _process();
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    release();
  }
}

void Connection::on_error(std::exception const &exce) {
  ftpp::logger(ftpp::Logger::ERROR,
               ftpp::Format("Connection error: {}") % exce.what());
  release();
}

void Connection::on_release() {
  delete this;
}

bool Connection::_receiveRequest() {
  assert(_state == REQUEST);
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
