/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 03:30:48 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/07 08:05:57 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exceptions/OSError.hpp>
#include <socket/Socket.hpp>

#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>

namespace ftpp {

int Socket::_create_socket(int domain, int type, int protocol) {
  int sockfd = ::socket(domain, type, protocol);
  if (__glibc_unlikely(sockfd == -1))
    throw OSError(errno, "socket");
  return sockfd;
}

Socket::Socket(int domain, int type, int protocol)
    : _sockfd(_create_socket(domain, type, protocol)) {
}

Socket::Socket(int sockfd) : _sockfd(sockfd) {
}

Socket::~Socket() {
  if (_sockfd != -1)
    close();
}

int Socket::get_sockfd() const {
  return _sockfd;
}

void Socket::bind(sockaddr const *addr, socklen_t addrlen) {
  if (__glibc_unlikely(::bind(_sockfd, addr, addrlen) == -1))
    throw OSError(errno, "bind");
}

void Socket::connect(sockaddr const *addr, socklen_t addrlen) {
  if (__glibc_unlikely(::connect(_sockfd, addr, addrlen) == -1))
    throw OSError(errno, "connect");
}

void Socket::listen(int backlog) {
  if (__glibc_unlikely(::listen(_sockfd, backlog) == -1))
    throw OSError(errno, "listen");
}

int Socket::accept(sockaddr *addr, socklen_t *addrlen) {
  int connfd = ::accept(_sockfd, addr, addrlen);
  if (__glibc_unlikely(connfd == -1))
    throw OSError(errno, "accept");
  return connfd;
}

std::size_t Socket::write(void const *buf, std::size_t len) {
  ssize_t ret = ::write(_sockfd, buf, len);
  if (__glibc_unlikely(ret == -1))
#if defined(FT_SUBJECT_NOT_COMPLIANT)
    throw OSError(errno, "write");
#else
    throw std::runtime_error("write: No access to error details");
#endif
  return ret;
}

std::size_t Socket::read(void *buf, std::size_t len) {
  ssize_t ret = ::read(_sockfd, buf, len);
  if (__glibc_unlikely(ret == -1))
#if defined(FT_SUBJECT_NOT_COMPLIANT)
    throw OSError(errno, "read");
#else
    throw std::runtime_error("read: No access to error details");
#endif
  return ret;
}

std::size_t Socket::send(void const *buf, std::size_t len, int flags) {
  ssize_t ret = ::send(_sockfd, buf, len, flags);
  if (__glibc_unlikely(ret == -1))
#if defined(FT_SUBJECT_NOT_COMPLIANT)
    throw OSError(errno, "send");
#else
    throw std::runtime_error("send: No access to error details");
#endif
  return ret;
}

std::size_t Socket::recv(void *buf, std::size_t len, int flags) {
  ssize_t ret = ::recv(_sockfd, buf, len, flags);
  if (__glibc_unlikely(ret == -1))
#if defined(FT_SUBJECT_NOT_COMPLIANT)
    throw OSError(errno, "recv");
#else
    throw std::runtime_error("recv: No access to error details");
#endif
  return ret;
}

std::size_t Socket::sendto(void const *buf, std::size_t len, int flags,
                           sockaddr const *dest_addr, socklen_t addrlen) {
#if defined(FT_SUBJECT_NOT_COMPLIANT)
  ssize_t ret = ::sendto(_sockfd, buf, len, flags, dest_addr, addrlen);
  if (__glibc_unlikely(ret == -1))
    throw OSError(errno, "sendto");
  return ret;
#else
  throw std::runtime_error("sendto: Forbidden Function");
#endif
}

std::size_t Socket::recvfrom(void *buf, std::size_t len, int flags,
                             sockaddr *src_addr, socklen_t *addrlen) {
#if defined(FT_SUBJECT_NOT_COMPLIANT)
  ssize_t ret = ::recvfrom(_sockfd, buf, len, flags, src_addr, addrlen);
  if (__glibc_unlikely(ret == -1))
    throw OSError(errno, "recvfrom");
  return ret;
#else
  throw std::runtime_error("recvfrom: Forbidden Function");
#endif
}

void Socket::close() {
  if (__glibc_unlikely(::close(_sockfd) == -1))
    throw OSError(errno, "close");
  _sockfd = -1;
}

void Socket::getsockopt(int level, int optname, void *optval,
                        socklen_t *optlen) {
#if defined(FT_SUBJECT_NOT_COMPLIANT)
  if (__glibc_unlikely(::getsockopt(_sockfd, level, optname, optval, optlen) ==
                       -1))
    throw OSError(errno, "getsockopt");
#else
  throw std::runtime_error("getsockopt: Forbidden Function");
#endif
}

void Socket::setsockopt(int level, int optname, void const *optval,
                        socklen_t optlen) {
  if (__glibc_unlikely(::setsockopt(_sockfd, level, optname, optval, optlen) ==
                       -1))
    throw OSError(errno, "setsockopt");
}

} // namespace ftpp
