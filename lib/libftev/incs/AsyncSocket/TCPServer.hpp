/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 07:11:15 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/04 09:05:14 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <AsyncSocket/BaseAsyncStreamConnection.hpp>
#include <AsyncSocket/BaseAsyncStreamServer.hpp>

#include <exceptions/OSError.hpp>
#include <list>
#include <netdb.h>

namespace ftev {

template <typename StreamServer> class TCPServer {
private:
  EventLoop &loop;
  std::list<BaseAsyncStreamServer *> _servers;

public:
  TCPServer(EventLoop &loop, char const *name, char const *service);
  ~TCPServer();

  void start();
};

template <typename StreamServer>
TCPServer<StreamServer>::TCPServer(EventLoop &loop, char const *name,
                                   char const *service)
    : loop(loop) {
  addrinfo hints = {};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  addrinfo *res;
  if (__glibc_unlikely(getaddrinfo(name, service, &hints, &res) == -1))
    throw ftpp::OSError(errno, "getaddrinfo");
  try {
    for (addrinfo *rp = res; rp != NULL; rp = rp->ai_next) {
      BaseAsyncStreamServer *server = new StreamServer(
          loop, rp->ai_family, rp->ai_socktype, rp->ai_protocol);
      try {
        server->bind(rp->ai_addr, rp->ai_addrlen);
        _servers.push_back(server);
      } catch (...) {
        delete server;
        throw;
      }
    }
  } catch (...) {
    freeaddrinfo(res);
    for (std::list<BaseAsyncStreamServer *>::iterator it = _servers.begin();
         it != _servers.end(); ++it)
      delete *it;
    throw;
  }
  freeaddrinfo(res);
}

template <typename StreamServer> TCPServer<StreamServer>::~TCPServer() {
  for (std::list<BaseAsyncStreamServer *>::iterator it = _servers.begin();
       it != _servers.end(); ++it)
    delete *it;
}

template <typename StreamServer> void TCPServer<StreamServer>::start() {
  for (std::list<BaseAsyncStreamServer *>::iterator it = _servers.begin();
       it != _servers.end(); ++it)
    (*it)->listen();
}

} // namespace ftev
