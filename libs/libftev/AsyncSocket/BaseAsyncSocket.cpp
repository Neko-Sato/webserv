/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseAsyncSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 23:59:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/11 19:05:54 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <AsyncSocket/BaseAsyncSocket.hpp>
#include <EventLoop.hpp>

#include <exceptions/OSError.hpp>
#include <macros.hpp>

#include <fcntl.h>

namespace ftev {

BaseAsyncSocket::BaseAsyncSocket(EventLoop &loop, int domain, int type,
                                 int protocol)
    : BaseIOWatcher(loop),
      _socket(domain, type | SOCK_NONBLOCK | SOCK_CLOEXEC, protocol) {
}

BaseAsyncSocket::BaseAsyncSocket(EventLoop &loop, ftpp::Socket &socket)
    : BaseIOWatcher(loop) {
  _socket.swap(socket);
  int sockfd = _socket.getSockfd();
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
}

BaseAsyncSocket::~BaseAsyncSocket() {
  if (is_active())
    stop();
}

void BaseAsyncSocket::bind(sockaddr const *addr, socklen_t addrlen) {
  _socket.bind(addr, addrlen);
}

} // namespace ftev
