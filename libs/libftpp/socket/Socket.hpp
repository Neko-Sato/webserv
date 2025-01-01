/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 03:30:48 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 15:56:33 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <move.hpp>

#include <map>
#include <sys/socket.h>
#include <vector>

namespace ftpp {

class Socket {
private:
  int _sockfd;

#if !defined(FT_SUBJECT_NOT_COMPLIANT)
  typedef std::vector<char> SockAddr;
  static std::map<int, SockAddr> _sockNames;
  static std::map<int, SockAddr> _peerNames;

#endif

  static int _create_socket(int domain, int type, int protocol);

  Socket(int sockfd);
  Socket(Socket const &rhs);
  Socket &operator=(Socket const &rhs);

public:
  Socket();
  Socket(int domain, int type, int protocol);
  Socket(transfer<Socket> const &rhs);
  ~Socket();
  Socket &operator=(transfer<Socket> const &rhs);
  void swap(Socket &rhs);

  int getSockfd() const;

  void bind(sockaddr const *addr, socklen_t addrlen);
  void connect(sockaddr const *addr, socklen_t addrlen);
  void listen(int backlog = 1024);

  Socket accept();
  std::size_t write(void const *buf, std::size_t len);
  std::size_t read(void *buf, std::size_t len);
  std::size_t send(void const *buf, std::size_t len, int flags);
  std::size_t recv(void *buf, std::size_t len, int flags);
  std::size_t sendto(void const *buf, std::size_t len, int flags,
                     sockaddr const *dest_addr, socklen_t addrlen);
  std::size_t recvfrom(void *buf, std::size_t len, int flags,
                       sockaddr *src_addr, socklen_t *addrlen);
  void close();

  void getsockname(sockaddr *addr, socklen_t *addrlen);
  void getpeername(sockaddr *addr, socklen_t *addrlen);

  void getsockopt(int level, int optname, void *optval, socklen_t *optlen);
  void setsockopt(int level, int optname, void const *optval, socklen_t optlen);
};

} // namespace ftpp
