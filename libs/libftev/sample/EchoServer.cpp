/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EchoServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 04:36:19 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/05 04:47:47 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EchoServer.hpp>

#include <cstring>
#include <iostream>
#include <netinet/in.h>

namespace ftev {

EchoServer::EchoServer(ftev::EventLoop &loop, char const *host, int port)
    : BaseTCPServer(loop) {
  _setup(host, port);
}

EchoServer::~EchoServer() {
}

EchoServer::Server *EchoServer::create_server(int domain, int type,
                                              int protocol) {
  return new Server(loop, domain, type, protocol);
}

EchoServer::Server::Server(ftev::EventLoop &loop, int domain, int type,
                           int protocol)
    : BaseAsyncSocket(loop, domain, type, protocol) {
}

EchoServer::Server::~Server() {
}

void EchoServer::Server::on_accept(int sockfd, sockaddr const *addr) {
  (void)addr;
  try {
    new Connection(loop, sockfd);
  } catch (std::exception const &e) {
    close(sockfd);
    std::cerr << "Connection Error: " << e.what() << std::endl;
  }
}

void EchoServer::Server::on_except() {
  std::cerr << "Server Error" << std::endl;
  loop.stop();
}

EchoServer::Connection::Connection(ftev::EventLoop &loop, int connfd)
    : BaseAsyncSocket(loop, connfd) {
}

EchoServer::Connection::~Connection() {
}

void EchoServer::Connection::on_data(std::deque<char> &data) {
  std::vector<char> buffer(data.begin(), data.end());
  data.clear();
  write(buffer.data(), buffer.size());
  std::cout.write(buffer.data(), buffer.size());
}

void EchoServer::Connection::on_eof(std::deque<char> &data) {
  on_data(data);
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

} // namespace ftev
