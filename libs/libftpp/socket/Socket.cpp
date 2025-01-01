/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 03:30:48 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 15:57:30 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exceptions/OSError.hpp>
#include <socket/Socket.hpp>

#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

namespace ftpp {

#if !defined(FT_SUBJECT_NOT_COMPLIANT)
std::map<int, Socket::SockAddr> Socket::_sockNames;
std::map<int, Socket::SockAddr> Socket::_peerNames;
#endif

int Socket::_create_socket(int domain, int type, int protocol) {
  int sockfd = ::socket(domain, type, protocol);
  if (__glibc_unlikely(sockfd == -1))
    throw OSError(errno, "socket");
  return sockfd;
}

Socket::Socket() : _sockfd(-1) {
}

Socket::Socket(int domain, int type, int protocol) {
  _sockfd = _create_socket(domain, type, protocol);
}

Socket::Socket(transfer<Socket> const &rhs) : _sockfd(rhs.ref._sockfd) {
  rhs.ref._sockfd = -1;
}

Socket::Socket(int sockfd) : _sockfd(sockfd) {
}

Socket::~Socket() {
  if (_sockfd != -1)
    close();
}

Socket &Socket::operator=(transfer<Socket> const &rhs) {
  if (this != &rhs.ref) {
    close();
    _sockfd = rhs.ref._sockfd;
    rhs.ref._sockfd = -1;
  }
  return *this;
}

void Socket::swap(Socket &rhs) {
  std::swap(_sockfd, rhs._sockfd);
}

int Socket::getSockfd() const {
  return _sockfd;
}

void Socket::bind(sockaddr const *addr, socklen_t addrlen) {
  if (__glibc_unlikely(::bind(_sockfd, addr, addrlen) == -1))
    throw OSError(errno, "bind");
#if !defined(FT_SUBJECT_NOT_COMPLIANT)
  char const *&data = reinterpret_cast<char const *&>(addr);
  _sockNames[_sockfd].assign(data, data + addrlen);
#endif
}

void Socket::connect(sockaddr const *addr, socklen_t addrlen) {
  if (__glibc_unlikely(::connect(_sockfd, addr, addrlen) == -1))
    throw OSError(errno, "connect");
#if !defined(FT_SUBJECT_NOT_COMPLIANT)
  char const *&data = reinterpret_cast<char const *&>(addr);
  _peerNames[_sockfd].assign(data, data + addrlen);
#endif
}

void Socket::listen(int backlog) {
  if (__glibc_unlikely(::listen(_sockfd, backlog) == -1))
    throw OSError(errno, "listen");
}

Socket Socket::accept() {
  sockaddr_storage addr;
  socklen_t addrlen = sizeof(addr);
  int connfd = ::accept(_sockfd, reinterpret_cast<sockaddr *>(&addr), &addrlen);
  if (__glibc_unlikely(connfd == -1))
    throw OSError(errno, "accept");
#if !defined(FT_SUBJECT_NOT_COMPLIANT)
  _sockNames[connfd] = _sockNames[_sockfd];
  char *&data = reinterpret_cast<char *&>(addr);
  _peerNames[connfd].assign(data, data + addrlen);
#endif
  return Socket(connfd);
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
  (void)buf;
  (void)len;
  (void)flags;
  (void)dest_addr;
  (void)addrlen;
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
  (void)buf;
  (void)len;
  (void)flags;
  (void)src_addr;
  (void)addrlen;
  throw std::runtime_error("recvfrom: Forbidden Function");
#endif
}

void Socket::close() {
  if (_sockfd != -1) {
    if (__glibc_unlikely(::close(_sockfd) == -1))
      throw OSError(errno, "close");
    _sockNames.erase(_sockfd);
    _peerNames.erase(_sockfd);
    _sockfd = -1;
  }
}

void Socket::getsockname(sockaddr *addr, socklen_t *addrlen) {
#if defined(FT_SUBJECT_NOT_COMPLIANT)
  if (__glibc_unlikely(::getsockname(_sockfd, addr, addrlen) == -1))
    throw OSError(errno, "getsockname");
#else
  std::map<int, SockAddr>::iterator it = _sockNames.find(_sockfd);
  if (it == _sockNames.end())
    throw std::runtime_error("getsockname: No Socket Name");
  std::memcpy(addr, it->second.data(), it->second.size());
  *addrlen = it->second.size();
#endif
}

void Socket::getpeername(sockaddr *addr, socklen_t *addrlen) {
#if defined(FT_SUBJECT_NOT_COMPLIANT)
  if (__glibc_unlikely(::getpeername(_sockfd, addr, addrlen) == -1))
    throw OSError(errno, "getpeername");
#else
  std::map<int, SockAddr>::iterator it = _peerNames.find(_sockfd);
  if (it == _peerNames.end())
    throw std::runtime_error("getpeername: No Peer Name");
  std::memcpy(addr, it->second.data(), it->second.size());
  *addrlen = it->second.size();
#endif
}

void Socket::getsockopt(int level, int optname, void *optval,
                        socklen_t *optlen) {
#if defined(FT_SUBJECT_NOT_COMPLIANT)
  if (__glibc_unlikely(::getsockopt(_sockfd, level, optname, optval, optlen) ==
                       -1))
    throw OSError(errno, "getsockopt");
#else
  (void)level;
  (void)optname;
  (void)optval;
  (void)optlen;
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
