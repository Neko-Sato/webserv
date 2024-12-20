/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseAsyncStreamServer.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 13:43:55 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/06 09:08:49 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <AsyncSocket/BaseAsyncSocket.hpp>

namespace ftev {

class BaseAsyncStreamServer : virtual public BaseAsyncSocket {
protected:
  BaseAsyncStreamServer();

public:
  virtual ~BaseAsyncStreamServer();
  void bind(sockaddr const *addr, socklen_t addrlen);
  void listen(int backlog = 1024);
  void on_read();
  void on_write();

  virtual void on_accept(int sockfd, sockaddr const *addr) = 0;
};

} // namespace ftev
