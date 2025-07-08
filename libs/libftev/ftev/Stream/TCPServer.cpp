/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 01:07:13 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/08 18:02:55 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Stream/TCPServer.hpp>

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/socket/AddrInfos.hpp>
#include <ftpp/socket/Socket.hpp>
#include <ftpp/string/string.hpp>
#include <ftpp/fcntl/fcntl.hpp>

#include <cassert>
#include <fcntl.h>

namespace ftev {

TCPServer::TCPServer(EventLoop &loop, const std::string &host, int port, int backlog)
    : loop(loop) {
  try {
    ftpp::AddrInfos::Hints hints(AF_UNSPEC, SOCK_STREAM, 0, AI_PASSIVE);
    ftpp::AddrInfos infos(host.c_str(), ftpp::to_string(port).c_str(), hints);
    for (ftpp::AddrInfos::iterator it = infos.begin(); it != infos.end();
         ++it) {
#if defined(SOCK_NONBLOCK)
      ftpp::Socket socket(it->ai_family, it->ai_socktype | SOCK_CLOEXEC,
                          it->ai_protocol);
#else
      ftpp::Socket socket(it->ai_family, it->ai_socktype, it->ai_protocol);
      ftpp::setcloexec(socket.getSockfd(), true);
#endif
      {
        int opt = 1;
        socket.setsockopt(SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
      }
      ftpp::setblocking(socket.getSockfd(), false);
      socket.bind(it->ai_addr, it->ai_addrlen);
      socket.listen(backlog);
      StreamServerTransport *transport =
          new StreamServerTransport(loop, *this, socket);
      try {
        _transports.push_back(transport);
      } catch (...) {
        delete transport;
        throw;
      }
    }
  } catch (...) {
    for (Transports::iterator it = _transports.begin(); it != _transports.end();
         it = _transports.erase(it))
      delete *it;
    throw;
  }
}

TCPServer::~TCPServer() {
  for (Transports::iterator it = _transports.begin(); it != _transports.end();
       it = _transports.erase(it))
    delete *it;
}

} // namespace ftev
