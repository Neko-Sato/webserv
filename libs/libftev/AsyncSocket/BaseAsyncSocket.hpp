/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseAsyncSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 23:59:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/11 19:05:29 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop/BaseIOWatcher.hpp>
#include <socket/Socket.hpp>

namespace ftev {

class BaseAsyncSocket : protected EventLoop::BaseIOWatcher {
protected:
  ftpp::Socket _socket;

  BaseAsyncSocket(EventLoop &loop, int domain, int type, int protocol);
  BaseAsyncSocket(EventLoop &loop, ftpp::Socket &socket);

public:
  virtual ~BaseAsyncSocket();

  void bind(sockaddr const *addr, socklen_t addrlen);
};

} // namespace ftev
