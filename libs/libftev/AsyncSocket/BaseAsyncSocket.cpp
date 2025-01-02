/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseAsyncSocket.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 23:59:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/02 20:26:20 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <AsyncSocket/BaseAsyncSocket.hpp>
#include <EventLoop.hpp>

#include <exceptions/OSError.hpp>

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
  if (__glibc_unlikely(flags == -1))
    throw ftpp::OSError(sockfd, "fcntl");
  if (__glibc_unlikely(fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1))
    throw ftpp::OSError(sockfd, "fcntl");
  flags = fcntl(sockfd, F_GETFD);
  if (__glibc_unlikely(flags == -1))
    throw ftpp::OSError(sockfd, "fcntl");
  if (__glibc_unlikely(fcntl(sockfd, F_SETFD, flags | FD_CLOEXEC) == -1))
    throw ftpp::OSError(sockfd, "fcntl");
}

BaseAsyncSocket::~BaseAsyncSocket() {
  if (is_active())
    stop();
}

} // namespace ftev
