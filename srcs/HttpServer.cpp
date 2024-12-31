/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:55:30 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 01:00:01 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"

#include <iostream>

HttpServer::HttpServer(ftev::EventLoop &loop, char const *host, int port,
                       Configs &configs)
    : ftev::BaseTCPServer(loop), _configs(configs) {
  _setup(host, port);
}

HttpServer::~HttpServer() {
}

HttpServer::Server *HttpServer::create_server(int domain, int type,
                                              int protocol) {
  return new Server(loop, domain, type, protocol, _configs);
}

HttpServer::Server::Server(ftev::EventLoop &loop, int domain, int type,
                           int protocol, Configs &configs)
    : ftev::BaseAsyncSocket(loop, domain, type, protocol), _configs(configs) {
}

HttpServer::Server::~Server() {
}

void HttpServer::Server::on_accept(int sockfd, sockaddr const *addr) {
  (void)addr;
  try {
    new Connection(loop, sockfd, _configs);
  } catch (std::exception const &e) {
    close(sockfd);
    std::cerr << "Connection Error: " << e.what() << std::endl;
  }
}

void HttpServer::Server::on_except() {
  std::cerr << "Server Error" << std::endl;
  loop.stop();
}

HttpServer::Connection::Connection(ftev::EventLoop &loop, int connfd,
                                   Configs &configs)
    : ftev::BaseAsyncSocket(loop, connfd), _configs(configs) {
}

HttpServer::Connection::~Connection() {
}

void HttpServer::Connection::on_data(std::deque<char> &data) {
  (void)data;
}

void HttpServer::Connection::on_eof(std::deque<char> &data) {
  (void)data;
}

void HttpServer::Connection::on_drain() {
}

void HttpServer::Connection::on_except() {
}

void HttpServer::Connection::on_release() {
  delete this;
}
