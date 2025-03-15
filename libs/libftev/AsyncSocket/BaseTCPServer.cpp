/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTCPServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 02:13:47 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/15 23:45:12 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <AsyncSocket/BaseTCPServer.hpp>
#include <EventLoop.hpp>

#include <Format.hpp>
#include <exceptions/OSError.hpp>
#include <ft_string.hpp>
#include <logger/Logger.hpp>
#include <socket/AddrInfos.hpp>

#include <cassert>
#include <cstring>
#include <iostream>

namespace ftev {

BaseTCPServer::BaseTCPServer(EventLoop &loop, std::string const &host, int port)
    : loop(loop) {
  try {
    ftpp::AddrInfos::Hints hints(AF_UNSPEC, SOCK_STREAM, 0, AI_PASSIVE);
    ftpp::AddrInfos infos(host.c_str(), ftpp::to_string(port).c_str(), hints);
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
    for (Servers::iterator it = _servers.begin(); it != _servers.end(); ++it)
      (*it)->listen();
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
  try {
    _socket.accept(conn);
  } catch (std::exception const &e) {
    ftpp::logger.log(ftpp::Logger::WARN,
                     ftpp::Format("TCPServer accept: {}") % e.what());
    return;
  }
  server.on_connect(conn);
}

void BaseTCPServer::Server::on_write() {
  assert(false);
}

void BaseTCPServer::Server::on_except() {
  {
    char const *msg;
#if defined(FT_SUBJECT_NOT_COMPLIANT)
    int error;
    socklen_t len = sizeof(error);
    _socket.getsockopt(SOL_SOCKET, SO_ERROR, &error, &len);
    if (error)
      msg = strerror(error);
    else
#else
    msg = "Unknown exception";
    ftpp::logger.log(ftpp::Logger::WARN, ftpp::Format("TCPServer: {}") % msg);
#endif
  }
  server._servers.remove(this);
  delete this;
}

} // namespace ftev
