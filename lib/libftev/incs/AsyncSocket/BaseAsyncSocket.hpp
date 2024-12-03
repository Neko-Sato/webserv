/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseAsyncSocket.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 23:59:53 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/02 06:26:57 by hshimizu         ###   ########.fr       */
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
  BaseAsyncSocket(EventLoop &loop, int sockfd);

public:
  virtual ~BaseAsyncSocket();
};

} // namespace ftev
