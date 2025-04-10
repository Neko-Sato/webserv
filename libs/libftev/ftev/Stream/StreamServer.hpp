/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StreamServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 00:47:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/01 22:53:42 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/IOWatcher.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>
#include <ftpp/socket/Socket.hpp>

namespace ftev {

struct StreamServerProtocol {
  virtual void on_connect(ftpp::Socket &conn) = 0;
};

class StreamServerTransport : private ftpp::NonCopyable {
private:
  class Handler : public EventLoop::IOWatcher {
  private:
    StreamServerTransport &_transport;

  public:
    Handler(EventLoop &loop, StreamServerTransport &transport);
    ~Handler();

    void on_read();
    void on_write();
    void on_except();
  };

  StreamServerProtocol &_protocol;
  ftpp::Socket _socket;
  Handler *_handler;
  bool _closed;

public:
  StreamServerTransport(EventLoop &loop, StreamServerProtocol &protocol,
                        ftpp::Socket &socket);
  ~StreamServerTransport();

  void close();
};

} // namespace ftev
