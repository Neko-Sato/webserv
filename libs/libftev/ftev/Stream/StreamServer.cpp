/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StreamServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:50:15 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/28 18:25:35 by hshimizu         ###   ########.fr       */
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

StreamServer::StreamServer(EventLoop &loop, ftpp::Socket &socket)
    : IOWatcher(loop) {
  _socket.swap(socket);
  int sockfd = _socket.getSockfd();
  setblocking(sockfd, false);
  start(sockfd, ftpp::Selector::READ);
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

void StreamServer::on_write() {
  assert(false);
}

void StreamServer::on_except() {
  assert(false);
}

} // namespace ftev
