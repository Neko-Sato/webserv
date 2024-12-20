/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTCPServer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 07:11:15 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/06 10:42:42 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <AsyncSocket/BaseTCPServer.hpp>
#include <socket/AddrInfos.hpp>

#include <cstdio>

namespace ftev {

BaseTCPServer::BaseTCPServer(EventLoop &loop) : loop(loop) {
}

BaseTCPServer::~BaseTCPServer() {
  for (Servers::iterator it = _servers.begin(); it != _servers.end();
       it = _servers.erase(it))
    delete *it;
}

void BaseTCPServer::_setup(char const *host, int port) {
  try {
    char service[12];
    snprintf(service, sizeof(service), "%d", port);
    ftpp::AddrInfos::Hints hints(AF_UNSPEC, SOCK_STREAM, 0, AI_PASSIVE);
    ftpp::AddrInfos info(host, service, hints);
    for (ftpp::AddrInfos::iterator it = info.begin(); it != info.end(); ++it) {
      BaseAsyncStreamServer *server =
          create_server(it->ai_family, it->ai_socktype, it->ai_protocol);
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

void BaseTCPServer::start() {
  for (Servers::iterator it = _servers.begin(); it != _servers.end(); ++it)
    (*it)->listen();
}

} // namespace ftev
