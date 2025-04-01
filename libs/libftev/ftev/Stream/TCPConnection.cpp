/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPConnection.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 01:47:05 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/02 00:05:00 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Stream/TCPConnection.hpp>
#include <ftev/utils/utils.hpp>

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/macros.hpp>
#include <ftpp/socket/AddrInfos.hpp>
#include <ftpp/socket/Socket.hpp>
#include <ftpp/string/string.hpp>

#include <cassert>
#include <fcntl.h>

namespace ftev {

TCPConnection::TCPConnection(EventLoop &loop, std::string const &host, int port)
    : loop(loop), _transport(NULL) {
  ftpp::AddrInfos::Hints hints(AF_UNSPEC, SOCK_STREAM, 0, AI_PASSIVE);
  ftpp::AddrInfos infos(host.c_str(), ftpp::to_string(port).c_str(), hints);
  for (ftpp::AddrInfos::iterator it = infos.begin(); it != infos.end(); ++it) {
    ftpp::Socket socket(it->ai_family, it->ai_socktype | SOCK_CLOEXEC,
                        it->ai_protocol);
    setblocking(socket.getSockfd(), false);
    try {
      socket.connect(it->ai_addr, it->ai_addrlen);
    } catch (ftpp::OSError const &e) {
      if (e.get_errno() != EINPROGRESS)
        continue;
    }
    _transport = new StreamConnectionTransport(loop, *this, socket);
    return;
  }
  throw std::runtime_error("TCPConnection");
}

TCPConnection::TCPConnection(EventLoop &loop, ftpp::Socket &socket)
    : loop(loop), _transport(NULL) {
  setblocking(socket.getSockfd(), false);
  _transport = new StreamConnectionTransport(loop, *this, socket);
}

TCPConnection::~TCPConnection() {
  delete _transport;
}

StreamConnectionTransport &TCPConnection::get_transport() {
  return *_transport;
}

} // namespace ftev
