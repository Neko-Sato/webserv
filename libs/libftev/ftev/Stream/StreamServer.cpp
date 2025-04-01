/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StreamServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 00:47:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/02 00:00:35 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Stream/StreamServer.hpp>
#include <ftev/utils/utils.hpp>

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/macros.hpp>

#include <cassert>
#include <fcntl.h>

namespace ftev {

StreamServerTransport::Handler::Handler(EventLoop &loop,
                                        StreamServerTransport &transport)
    : IOWatcher(loop), _transport(transport) {
  start(transport._socket.getSockfd(), ftpp::Selector::READ);
}

StreamServerTransport::Handler::~Handler() {
  if (is_active())
    stop();
}

void StreamServerTransport::Handler::on_read() {
  ftpp::Socket conn;
  try {
    _transport._socket.accept(conn);
    int sockfd = conn.getSockfd();
    int flags = fcntl(sockfd, F_GETFD);
    if (unlikely(flags == -1))
      throw ftpp::OSError(sockfd, "fcntl");
    if (unlikely(fcntl(sockfd, F_SETFD, flags | FD_CLOEXEC) == -1))
      throw ftpp::OSError(sockfd, "fcntl");
  } catch (std::exception const &e) {
    ftpp::logger(ftpp::Logger::ERROR,
                 ftpp::Format("StreamServer: {}") % e.what());
    return;
  }
  _transport._protocol.on_connect(conn);
}

void StreamServerTransport::Handler::on_write() {
  assert(false);
}

void StreamServerTransport::Handler::on_except() {
  assert(false);
}

StreamServerTransport::StreamServerTransport(EventLoop &loop,
                                             StreamServerProtocol &protocol,
                                             ftpp::Socket &socket)
    : _protocol(protocol), _handler(NULL), _closed(false) {
  _socket.swap(socket);
  _handler = new Handler(loop, *this);
}

StreamServerTransport::~StreamServerTransport() {
  delete _handler;
}

void StreamServerTransport::close() {
  if (_closed)
    throw std::runtime_error("already closed");
  if (_handler->is_active())
    _handler->stop();
  _socket.close();
  _closed = true;
}

} // namespace ftev
