/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTCPServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 02:13:47 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/11 19:05:39 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <AsyncSocket/BaseTCPServer.hpp>
#include <EventLoop.hpp>

#include <socket/AddrInfos.hpp>

#include <cassert>
#include <cstdio>

namespace ftev {

BaseTCPServer::BaseTCPServer(EventLoop &loop, std::string const &host, int port)
    : loop(loop) {
  char service[16];
  std::snprintf(service, sizeof(service), "%d", port);
  ftpp::AddrInfos::Hints hints(AF_UNSPEC, SOCK_STREAM, 0, AI_PASSIVE);
  ftpp::AddrInfos infos(host.c_str(), service, hints);
  try {
    for (ftpp::AddrInfos::iterator it = infos.begin(); it != infos.end();
         ++it) {
      Server *server =
          new Server(*this, it->ai_family, it->ai_socktype, it->ai_protocol);
      try {
        server->bind(it->ai_addr, it->ai_addrlen);
        _servers.push_back(server);
      } catch (...) {
        delete server;
        throw;
      }
    }
  } catch (...) {
    for (Servers::iterator it = _servers.begin(); it != _servers.end();
         it = _servers.erase(it))
      delete *it;
    throw;
  }
}

BaseTCPServer::~BaseTCPServer() {
  for (Servers::iterator it = _servers.begin(); it != _servers.end();
       it = _servers.erase(it))
    delete *it;
}

void BaseTCPServer::start() {
  for (Servers::iterator it = _servers.begin(); it != _servers.end(); ++it)
    (*it)->listen();
}

BaseTCPServer::Server::Server(BaseTCPServer &server, int domain, int type,
                              int protocol)
    : BaseAsyncSocket(server.loop, domain, type, protocol), server(server) {
  int opt = 1;
  _socket.setsockopt(SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

BaseTCPServer::Server::~Server() {
}

void BaseTCPServer::Server::listen(int backlog) {
  _socket.listen(backlog);
  start(_socket.getSockfd(), ftpp::BaseSelector::READ);
}

void BaseTCPServer::Server::on_read() {
  ftpp::Socket conn;
  _socket.accept(conn);
  server.on_connect(conn);
}

void BaseTCPServer::Server::on_write() {
  assert(false);
}

void BaseTCPServer::Server::on_except() {
  server._servers.remove(this);
  delete this;
}

} // namespace ftev
