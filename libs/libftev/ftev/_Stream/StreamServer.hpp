/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StreamServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:50:15 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/28 22:44:15 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/IOWatcher.hpp>

#include <ftpp/socket/Socket.hpp>

namespace ftev {

class StreamServer : private EventLoop::IOWatcher {
private:
  ftpp::Socket _socket;

protected:
  StreamServer(EventLoop &loop, ftpp::Socket &socket);

public:
  virtual ~StreamServer();

  using IOWatcher::loop;

  void on_read();
  void on_write();
  void on_except();

  virtual void on_connect(ftpp::Socket &conn) = 0;
};

}; // namespace ftev
