/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 03:30:48 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/30 06:24:45 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/socket.h>

namespace ftpp {

class Socket {
private:
  int _sockfd;

  static int _create_socket(int domain, int type, int protocol);

  Socket(Socket const &rhs);
  Socket &operator=(Socket const &rhs);

public:
  Socket(int domain, int type, int protocol);
  Socket(int sockfd);
  ~Socket();

  int get_sockfd() const;

  void bind(sockaddr const *addr, socklen_t addrlen);
  void connect(sockaddr const *addr, socklen_t addrlen);
  void listen(int backlog = 1024);
  int accept(sockaddr *addr, socklen_t *addrlen);

  std::size_t write(void const *buf, std::size_t len);
  std::size_t read(void *buf, std::size_t len);
  std::size_t send(void const *buf, std::size_t len, int flags);
  std::size_t recv(void *buf, std::size_t len, int flags);
#if not defined(FT_SUBJECT_COMPLIANT)
  std::size_t sendto(void const *buf, std::size_t len, int flags,
                     sockaddr const *dest_addr, socklen_t addrlen);
  std::size_t recvfrom(void *buf, std::size_t len, int flags,
                       sockaddr *src_addr, socklen_t *addrlen);
#endif

  void close();

  void getsockopt(int level, int optname, void *optval, socklen_t *optlen);
  void setsockopt(int level, int optname, void const *optval, socklen_t optlen);
};

} // namespace ftpp
