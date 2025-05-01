/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StreamServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 00:47:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/02 00:39:21 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/IOWatcher.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>
#include <ftpp/socket/Socket.hpp>

namespace ftev {

struct StreamServerProtocol {
  virtual ~StreamServerProtocol();
  virtual void onConnect(ftpp::Socket &conn) = 0;
};

class StreamServerTransport : private ftpp::NonCopyable {
private:
  class Handler : public EventLoop::IOWatcher {
  private:
    StreamServerTransport &_transport;

  public:
    Handler(EventLoop &loop, StreamServerTransport &transport);
    ~Handler();

    void onRead();
    void onWrite();
    void onExcept();
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
