/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPConnection.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 01:47:05 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/08 18:03:58 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Stream/TCPConnection.hpp>

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/socket/AddrInfos.hpp>
#include <ftpp/socket/Socket.hpp>
#include <ftpp/string/string.hpp>
#include <ftpp/fcntl/fcntl.hpp>

#include <cassert>
#include <fcntl.h>

namespace ftev {

TCPConnection::TCPConnection(EventLoop &loop, std::string const &host, int port)
    : loop(loop), _transport(NULL) {
  ftpp::AddrInfos::Hints hints(AF_UNSPEC, SOCK_STREAM, 0, AI_PASSIVE);
  ftpp::AddrInfos infos(host.c_str(), ftpp::to_string(port).c_str(), hints);
  for (ftpp::AddrInfos::iterator it = infos.begin(); it != infos.end(); ++it) {
#if defined(SOCK_NONBLOCK)
    ftpp::Socket socket(it->ai_family, it->ai_socktype | SOCK_CLOEXEC,
                        it->ai_protocol);
#else
    ftpp::Socket socket(it->ai_family, it->ai_socktype, it->ai_protocol);
    ftpp::setcloexec(socket.getSockfd(), true);
#endif
    ftpp::setblocking(socket.getSockfd(), false);
    try {
      socket.connect(it->ai_addr, it->ai_addrlen);
    } catch (ftpp::OSError const &e) {
      if (e.getErrno() != EINPROGRESS)
        continue;
    }
    _transport = new StreamConnectionTransport(loop, *this, socket);
    return;
  }
  throw std::runtime_error("TCPConnection");
}

TCPConnection::TCPConnection(EventLoop &loop, ftpp::Socket &socket)
    : loop(loop), _transport(NULL) {
  ftpp::setblocking(socket.getSockfd(), false);
  _transport = new StreamConnectionTransport(loop, *this, socket);
}

TCPConnection::~TCPConnection() {
  delete _transport;
}

StreamConnectionTransport &TCPConnection::getTransport() {
  return *_transport;
}

} // namespace ftev
