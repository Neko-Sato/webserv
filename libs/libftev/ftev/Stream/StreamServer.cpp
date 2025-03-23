/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StreamServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:50:15 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 06:09:33 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Stream/StreamServer.hpp>

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/macros.hpp>

#include <cassert>
#include <fcntl.h>

namespace ftev {

StreamServer::StreamServer(EventLoop &loop, ftpp::Socket &socket)
    : IOWatcher(loop) {
  _socket.swap(socket);
  int fd = _socket.getSockfd();
  int flags;
  flags = fcntl(fd, F_GETFL);
  if (unlikely(flags == -1))
    throw ftpp::OSError(fd, "fcntl");
  if (unlikely(fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1))
    throw ftpp::OSError(fd, "fcntl");
  start(fd, ftpp::Selector::READ);
}

StreamServer::~StreamServer() {
  if (is_active())
    stop();
}

void StreamServer::on_read() {
  ftpp::Socket conn;
  try {
    _socket.accept(conn);
    int sockfd = conn.getSockfd();
    int flags;
    flags = fcntl(sockfd, F_GETFL);
    if (unlikely(flags == -1))
      throw ftpp::OSError(sockfd, "fcntl");
    if (unlikely(fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1))
      throw ftpp::OSError(sockfd, "fcntl");
    flags = fcntl(sockfd, F_GETFD);
    if (unlikely(flags == -1))
      throw ftpp::OSError(sockfd, "fcntl");
    if (unlikely(fcntl(sockfd, F_SETFD, flags | FD_CLOEXEC) == -1))
      throw ftpp::OSError(sockfd, "fcntl");
  } catch (std::exception const &e) {
    ftpp::logger(ftpp::Logger::ERROR,
                 ftpp::Format("StreamServer: {}") % e.what());
    return;
  }
  on_connect(conn);
}

void StreamServer::on_except() {
#if defined(FT_SUBJECT_NOT_COMPLIANT)
  int error = 0;
  socklen_t len = sizeof(error);
  _socket.getsockopt(SOL_SOCKET, SO_ERROR, &error, &len);
  if (error)
    on_error(ftpp::OSError(error));
  else
#endif
    on_error(std::runtime_error("unkown error"));
}

void StreamServer::on_write() {
  assert(false);
}

} // namespace ftev
