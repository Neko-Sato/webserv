/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 03:30:48 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 07:22:11 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/macros.hpp>
#include <ftpp/socket/Socket.hpp>

#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

namespace ftpp {

int Socket::_create_socket(int domain, int type, int protocol) {
  int sockfd = ::socket(domain, type, protocol);
  if (unlikely(sockfd == -1))
    throw OSError(errno, "socket");
  return sockfd;
}

Socket::Socket() : _sockfd(-1) {
}

Socket::Socket(int domain, int type, int protocol) {
  _sockfd = _create_socket(domain, type, protocol);
}

Socket::Socket(int sockfd) : _sockfd(sockfd) {
}

Socket::~Socket() {
  if (_sockfd != -1)
    close();
}

void Socket::swap(Socket &rhs) throw() {
  std::swap(_sockfd, rhs._sockfd);
}

int Socket::getSockfd() const {
  return _sockfd;
}

void Socket::bind(sockaddr const *addr, socklen_t addrlen) {
  if (unlikely(::bind(_sockfd, addr, addrlen) == -1))
    throw OSError(errno, "bind");
}

void Socket::connect(sockaddr const *addr, socklen_t addrlen) {
  if (unlikely(::connect(_sockfd, addr, addrlen) == -1))
    throw OSError(errno, "connect");
}

void Socket::listen(int backlog) {
  if (unlikely(::listen(_sockfd, backlog) == -1))
    throw OSError(errno, "listen");
}

void Socket::accept(Socket &conn) {
  int connfd = ::accept(_sockfd, NULL, 0);
  if (unlikely(connfd == -1))
    throw OSError(errno, "accept");
  Socket(connfd).swap(conn);
}

std::size_t Socket::write(void const *buf, std::size_t len) {
  ssize_t ret = ::write(_sockfd, buf, len);
  if (unlikely(ret == -1))
#if defined(FT_SUBJECT_NOT_COMPLIANT)
    throw OSError(errno, "write");
#else
    throw std::runtime_error("write: No access to error details");
#endif
  return ret;
}

std::size_t Socket::read(void *buf, std::size_t len) {
  ssize_t ret = ::read(_sockfd, buf, len);
  if (unlikely(ret == -1))
#if defined(FT_SUBJECT_NOT_COMPLIANT)
    throw OSError(errno, "read");
#else
    throw std::runtime_error("read: No access to error details");
#endif
  return ret;
}

std::size_t Socket::send(void const *buf, std::size_t len, int flags) {
  ssize_t ret = ::send(_sockfd, buf, len, flags);
  if (unlikely(ret == -1))
#if defined(FT_SUBJECT_NOT_COMPLIANT)
    throw OSError(errno, "send");
#else
    throw std::runtime_error("send: No access to error details");
#endif
  return ret;
}

std::size_t Socket::recv(void *buf, std::size_t len, int flags) {
  ssize_t ret = ::recv(_sockfd, buf, len, flags);
  if (unlikely(ret == -1))
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
  if (unlikely(ret == -1))
    throw OSError(errno, "sendto");
  return ret;
#else
  UNUSED(buf);
  UNUSED(len);
  UNUSED(flags);
  UNUSED(dest_addr);
  UNUSED(addrlen);
  throw std::runtime_error("sendto: Forbidden Function");
#endif
}

std::size_t Socket::recvfrom(void *buf, std::size_t len, int flags,
                             sockaddr *src_addr, socklen_t *addrlen) {
#if defined(FT_SUBJECT_NOT_COMPLIANT)
  ssize_t ret = ::recvfrom(_sockfd, buf, len, flags, src_addr, addrlen);
  if (unlikely(ret == -1))
    throw OSError(errno, "recvfrom");
  return ret;
#else
  UNUSED(buf);
  UNUSED(len);
  UNUSED(flags);
  UNUSED(src_addr);
  UNUSED(addrlen);
  throw std::runtime_error("recvfrom: Forbidden Function");
#endif
}

void Socket::close() {
  if (_sockfd != -1) {
    if (unlikely(::close(_sockfd) == -1))
      throw OSError(errno, "close");
    _sockfd = -1;
  }
}

void Socket::shutdown(int how) {
#if defined(FT_SUBJECT_NOT_COMPLIANT)
  if (unlikely(::shutdown(_sockfd, how) == -1))
    throw OSError(errno, "shutdown");
#else
  UNUSED(how);
  throw std::runtime_error("shutdown: Forbidden Function");
#endif
}

void Socket::getsockname(sockaddr *addr, socklen_t *addrlen) {
#if defined(FT_SUBJECT_NOT_COMPLIANT)
  if (unlikely(::getsockname(_sockfd, addr, addrlen) == -1))
    throw OSError(errno, "getsockname");
#else
  UNUSED(addr);
  UNUSED(addrlen);
  throw std::runtime_error("getsockname: Forbidden Function");
#endif
}

void Socket::getpeername(sockaddr *addr, socklen_t *addrlen) {
#if defined(FT_SUBJECT_NOT_COMPLIANT)
  if (unlikely(::getpeername(_sockfd, addr, addrlen) == -1))
    throw OSError(errno, "getpeername");
#else
  UNUSED(addr);
  UNUSED(addrlen);
  throw std::runtime_error("getpeername: Forbidden Function");
#endif
}

void Socket::getsockopt(int level, int optname, void *optval,
                        socklen_t *optlen) {
#if defined(FT_SUBJECT_NOT_COMPLIANT)
  if (unlikely(::getsockopt(_sockfd, level, optname, optval, optlen) == -1))
    throw OSError(errno, "getsockopt");
#else
  UNUSED(level);
  UNUSED(optname);
  UNUSED(optval);
  UNUSED(optlen);
  throw std::runtime_error("getsockopt: Forbidden Function");
#endif
}

void Socket::setsockopt(int level, int optname, void const *optval,
                        socklen_t optlen) {
  if (unlikely(::setsockopt(_sockfd, level, optname, optval, optlen) == -1))
    throw OSError(errno, "setsockopt");
}

} // namespace ftpp

namespace std {

void swap(ftpp::Socket &lhs, ftpp::Socket &rhs) throw() {
  lhs.swap(rhs);
}

} // namespace std
