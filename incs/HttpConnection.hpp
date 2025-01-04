/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConnection.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 23:06:24 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/04 23:38:10 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "HttpServer.hpp"

#include <AsyncSocket/BaseAsyncSocket.hpp>
#include <AsyncSocket/MixinReader.hpp>
#include <AsyncSocket/MixinWriter.hpp>

#include <map>
#include <string>

class HttpConnection : virtual public ftev::BaseAsyncSocket,
                       public ftev::MixinReader,
                       public ftev::MixinWriter {
public:
  enum State {
    ERROR,
    REQUEST,
    HEADER,
    RESPONSE,
  };

  typedef std::multimap<std::string, std::string> Headers;

private:
  HttpServer const &_server;
  State _state;
  std::deque<char> _buffer;

  std::string _method;
  std::string _path;
  std::string _version;
  Headers _headers;

  void _parseRequest(std::string const &line);
  void _parseHeader(std::string const &line);
  void _sendResponse();

public:
  HttpConnection(ftev::EventLoop &loop, ftpp::Socket &socket,
                 HttpServer const &server);
  ~HttpConnection();

  void on_data(std::vector<char> const &data);
  void on_eof();
  void on_drain();
  void on_except();
  void on_release();
};
