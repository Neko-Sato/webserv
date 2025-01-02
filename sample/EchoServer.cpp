/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EchoServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 02:25:07 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/03 02:25:10 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EchoServer.hpp"

#include <cstring>
#include <iostream>
#include <netinet/in.h>

EchoServer::EchoServer(ftev::EventLoop &loop, char const *host, int port)
    : BaseTCPServer(loop, host, port) {
}

EchoServer::~EchoServer() {
}

void EchoServer::on_connect(ftpp::Socket &socket) {
  new Connection(*this, socket);
}

EchoServer::Connection::Connection(EchoServer &server, ftpp::Socket &socket)
    : BaseAsyncSocket(server.loop, socket) {
  start(_socket.getSockfd(), ftpp::BaseSelector::READ);
}

EchoServer::Connection::~Connection() {
}

void EchoServer::Connection::on_data(std::vector<char> const &data) {
  std::cout << "Received: ";
  std::cout.write(data.data(), data.size());
  std::cout << std::endl;
  _socket.write(data.data(), data.size());
}

void EchoServer::Connection::on_eof() {
  std::cout << "Connection closed" << std::endl;
  drain();
}

void EchoServer::Connection::on_drain() {
  if (is_active())
    stop();
  delete_later();
}

void EchoServer::Connection::on_except() {
  std::cerr << "Connection Error" << std::endl;
  stop();
  delete_later();
}

void EchoServer::Connection::on_release() {
  delete this;
}
