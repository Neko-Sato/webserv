/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConnection.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 23:10:39 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/04 23:44:47 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpConnection.hpp"
#include "constant.hpp"

#include <ft_string.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>

HttpConnection::HttpConnection(ftev::EventLoop &loop, ftpp::Socket &socket,
                               HttpServer const &server)
    : ftev::BaseAsyncSocket(loop, socket), _server(server), _state(REQUEST) {
  start(_socket.getSockfd(), ftpp::BaseSelector::READ);
  (void)_server;
}

HttpConnection::~HttpConnection() {
}

void HttpConnection::on_data(std::vector<char> const &data) {
  _buffer.insert(_buffer.end(), data.begin(), data.end());
  for (;;) {
    switch (_state) {
    case REQUEST:
    case HEADER:
      while (_state == REQUEST || _state == HEADER) {
        std::deque<char>::iterator it = std::search(
            _buffer.begin(), _buffer.end(), CRLF.begin(), CRLF.end());
        if (it == _buffer.end())
          return;
        std::string line(_buffer.begin(), it);
        _buffer.erase(_buffer.begin(), it + CRLF.size());
        if (_state == REQUEST)
          _parseRequest(line);
        else
          _parseHeader(line);
      }
      break;
    case RESPONSE:
      _sendResponse();
      return;
    case ERROR:
      stop();
      delete_later();
      return;
    }
  }
}

void HttpConnection::on_eof() {
  std::cout << "on_eof" << std::endl;
}

void HttpConnection::on_drain() {
  std::cout << "on_drain" << std::endl;
}

void HttpConnection::on_except() {
  std::cout << "on_except" << std::endl;
}

void HttpConnection::on_release() {
  std::cout << "on_release" << std::endl;
  delete this;
}

void HttpConnection::_parseRequest(std::string const &line) {
  std::stringstream ss(line);
  ss >> _method >> _path >> _version;
  if (ss.fail())
    _state = ERROR;
  else
    _state = HEADER;
}

void HttpConnection::_parseHeader(std::string const &line) {
  if (line.empty()) {
    _state = RESPONSE;
    return;
  }
  std::size_t pos = line.find(':');
  if (pos == std::string::npos)
    _state = ERROR;
  else
    _headers.insert(std::make_pair(ftpp::strtrim(line.substr(0, pos)),
                                   ftpp::strtrim(line.substr(pos + 1))));
}

void HttpConnection::_sendResponse() {
  Configs const &configs = _server.getConfigs();
  // std::string const *hostname = _getHostname();
  // hostnameを参考にして、ServerConfを取得する
  // ServerConf const &serverConf = _getServerConf();
  // serverConfを参考にして、Locationを取得する
  // Location const *location = _getLocation();
  // locationがタスクを生成してそれに残りのタスクを渡す
  // 
}
