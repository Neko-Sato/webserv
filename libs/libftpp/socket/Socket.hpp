/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 03:30:48 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/05 21:34:54 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <sys/socket.h>
#include <vector>

namespace ftpp {

class Socket {
private:
  int _sockfd;

  static int _create_socket(int domain, int type, int protocol);

  Socket(int sockfd);
  Socket(Socket const &rhs);
  Socket &operator=(Socket const &rhs);

public:
  Socket();
  Socket(int domain, int type, int protocol);
  ~Socket();
  void swap(Socket &rhs);

  int getSockfd() const;

  void bind(sockaddr const *addr, socklen_t addrlen);
  void connect(sockaddr const *addr, socklen_t addrlen);
  void listen(int backlog = 1024);

  void accept(Socket &conn);
  std::size_t write(void const *buf, std::size_t len);
  std::size_t read(void *buf, std::size_t len);
  std::size_t send(void const *buf, std::size_t len, int flags);
  std::size_t recv(void *buf, std::size_t len, int flags);
  std::size_t sendto(void const *buf, std::size_t len, int flags,
                     sockaddr const *dest_addr, socklen_t addrlen);
  std::size_t recvfrom(void *buf, std::size_t len, int flags,
                       sockaddr *src_addr, socklen_t *addrlen);
  void close();
  void shutdown(int how);

  void getsockname(sockaddr *addr, socklen_t *addrlen);
  void getpeername(sockaddr *addr, socklen_t *addrlen);

  void getsockopt(int level, int optname, void *optval, socklen_t *optlen);
  void setsockopt(int level, int optname, void const *optval, socklen_t optlen);
};

} // namespace ftpp
