/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConnection.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 23:06:24 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/03 01:52:56 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "HttpServer.hpp"

#include <AsyncSocket/BaseAsyncSocket.hpp>
#include <AsyncSocket/MixinReader.hpp>
#include <AsyncSocket/MixinWriter.hpp>

#include <string>

class HttpConnection : virtual public ftev::BaseAsyncSocket,
                       public ftev::MixinReader,
                       public ftev::MixinWriter {
public:
  enum State {
    REQUEST,
    HEADER,
  };

private:
  HttpServer const &_server;
  State _state;
  std::deque<char> _buffer;

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
