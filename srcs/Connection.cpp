/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:41:18 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/08 02:13:17 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "constants.hpp"

#include <ft_string.hpp>
#include <urllib/URI.hpp>
#include <urllib/urlquote.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>

Connection::Connection(ftev::EventLoop &loop, ftpp::Socket &socket,
                       Server const &server)
    : ftev::BaseTCPConnection(loop, socket), _server(server), _state(REQUEST) {
}

Connection::~Connection() {
}

void Connection::on_data(std::vector<char> const &data) {
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
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
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

void Connection::on_eof() {
  std::cout << "on_eof" << std::endl;
}

void Connection::on_drain() {
  switch (_state) {
  case RESPONSE:
    stop();
    delete_later();
    break;
  default:
    break;
  }
}

void Connection::on_except() {
  std::cout << "on_except" << std::endl;
}

void Connection::on_release() {
  std::cout << "on_release" << std::endl;
  delete this;
}

void Connection::_parseRequest(std::string const &line) {
  std::stringstream ss(line);
  std::string path;
  ss >> _method >> path >> _version;
  if (ss.fail()) {
    _state = ERROR;
    return;
  } else {
    _state = HEADER;
    ftpp::URI(ftpp::urlunquote(path)).swap(_uri);
  }
}

void Connection::_parseHeader(std::string const &line) {
  if (line.empty()) {
    _state = RESPONSE;
    _makeResponseTask();
    return;
  }
  std::size_t pos = line.find(':');
  if (pos == std::string::npos)
    _state = ERROR;
  else
    _headers[ftpp::tolower(ftpp::strtrim(line.substr(0, pos)))].push_back(
        ftpp::tolower(ftpp::strtrim(line.substr(pos + 1))));
}

void Connection::_makeResponseTask() {
  Headers::const_iterator it = _headers.find("host");
  ServerConf const &serverConf = _server.getConfigs().findServer(
      _server.getAddress(), it != _headers.end() ? it->second.back() : "");
  ServerConf::Locations::const_iterator jt =
      serverConf.findLocation(_method, _uri.getPath());
  if (jt == serverConf.getLocations().end()) {
    std::cerr << "404 Not Found" << std::endl;
  } else {
   std::cerr << typeid(jt->second.getDetail()).name() << std::endl;
  }
}

void Connection::_sendResponse() {
  std::stringstream aa;
  aa << "scheme: " << _uri.getScheme() << std::endl;
  aa << "netloc: " << _uri.getNetloc() << std::endl;
  aa << "path: " << _uri.getPath() << std::endl;
  aa << "query: " << _uri.getQuery() << std::endl;
  aa << "fragment: " << _uri.getFragment() << std::endl;
  for (Headers::const_iterator it = _headers.begin(); it != _headers.end();
       ++it) {
    aa << it->first << ": ";
    for (std::vector<std::string>::const_iterator jt = it->second.begin();
         jt != it->second.end(); ++jt) {
      if (jt != it->second.begin())
        aa << ", ";
      aa << *jt;
    }
    aa << std::endl;
  }
  std::stringstream ss;
  ss << "HTTP/1.1 200 OK" << CRLF;
  ss << "Content-Type: text/plain" << CRLF;
  ss << "Content-Length: " << aa.str().size() << CRLF;
  ss << CRLF;
  ss << aa.str();
  std::string const &response = ss.str();
  write(response.c_str(), response.size());
  drain();
}
