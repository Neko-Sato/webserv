/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPConnection.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 02:03:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 06:47:23 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Stream/TCPConnection.hpp>

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

TCPConnection::Handler::Handler(EventLoop &loop, ftpp::Socket &socket,
                                TCPConnection &connection)
    : StreamConnection(loop, socket), _connection(connection) {
}

TCPConnection::Handler::~Handler() {
}

void TCPConnection::Handler::on_data(std::vector<char> const &data) {
  _connection.on_data(data);
}

void TCPConnection::Handler::on_eof() {
  _connection.on_eof();
}

void TCPConnection::Handler::on_drain() {
  _connection.on_drain();
}

void TCPConnection::Handler::on_error(std::exception const &exce) {
  _connection.on_error(exce);
  _connection._handler = NULL;
  release();
}

void TCPConnection::Handler::on_release() {
  delete this;
}

TCPConnection::TCPConnection(EventLoop &loop, std::string const &host, int port)
    : loop(loop), _handler(NULL) {
  ftpp::AddrInfos::Hints hints(AF_UNSPEC, SOCK_STREAM, 0, AI_PASSIVE);
  ftpp::AddrInfos infos(host.c_str(), ftpp::to_string(port).c_str(), hints);
  for (ftpp::AddrInfos::iterator it = infos.begin(); it != infos.end(); ++it) {
    ftpp::Socket socket(it->ai_family, it->ai_socktype, it->ai_protocol);
    {
      int sockfd = socket.getSockfd();
      int flags = fcntl(sockfd, F_GETFD);
      if (unlikely(flags == -1))
        throw ftpp::OSError(sockfd, "fcntl");
      if (unlikely(fcntl(sockfd, F_SETFD, flags | FD_CLOEXEC) == -1))
        throw ftpp::OSError(sockfd, "fcntl");
    }
    try {
      socket.connect(it->ai_addr, it->ai_addrlen);
    } catch (ftpp::OSError const &e) {
      if (e.get_errno() != EINPROGRESS)
        continue;
    }
    _handler = new Handler(loop, socket, *this);
    return;
  }
  throw std::runtime_error("TCPConnection");
}

TCPConnection::~TCPConnection() {
  delete _handler;
}

TCPConnection::Handler &TCPConnection::getHandler() {
  assert(_handler);
  return *_handler;
}

} // namespace ftev
