/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StreamServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:50:15 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 04:25:50 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/Watchers/IOWatcher.hpp>

#include <ftpp/socket/Socket.hpp>

namespace ftev {

class StreamServer : private EventLoop::IOWatcher {
private:
  ftpp::Socket _socket;

public:
  StreamServer(EventLoop &loop, ftpp::Socket &socket);
  virtual ~StreamServer();

  using IOWatcher::loop;
  using IOWatcher::release;

  void on_read();
  void on_write();
  void on_except();

  virtual void on_connect(ftpp::Socket &conn) = 0;
  virtual void on_error(std::exception const &exce) = 0;
};

}; // namespace ftev