/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpConnection.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 23:10:39 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/03 22:07:54 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpConnection.hpp"

#include <algorithm>
#include <iostream>

HttpConnection::HttpConnection(ftev::EventLoop &loop, ftpp::Socket &socket,
                               HttpServer const &server)
    : ftev::BaseAsyncSocket(loop, socket), _server(server), _state(REQUEST) {
  start(_socket.getSockfd(), ftpp::BaseSelector::READ);
  (void)_server;
}

HttpConnection::~HttpConnection() {
}

void HttpConnection::on_data(std::vector<char> const &data) {
  std::cout << "on_data" << std::endl;
  std::cout.write(data.data(), data.size());
  switch (_state) {
  case REQUEST:
    break;
  case HEADER:
    break;
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
