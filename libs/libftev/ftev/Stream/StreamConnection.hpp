/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StreamTransport.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:50:15 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/31 18:05:43 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/DeferWatcher.hpp>
#include <ftev/EventLoop/IOWatcher.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>
#include <ftpp/socket/Socket.hpp>

namespace ftev {

struct StreamConnectionProtocol {
  virtual ~StreamConnectionProtocol();
  virtual void onData(std::vector<char> const &data) = 0;
  virtual void onEof() = 0;
  virtual void onDrain() = 0;
  virtual void onError(std::exception const &exce) = 0;
};

class StreamConnectionTransport : private ftpp::NonCopyable {
private:
  class Handler : public EventLoop::IOWatcher {
  private:
    StreamConnectionTransport &_transport;

  public:
    Handler(EventLoop &loop, StreamConnectionTransport &transport);
    ~Handler();

    void onRead();
    void onWrite();
    void onExcept();
  };

  class DrainHandler : public EventLoop::DeferWatcher {
  private:
    StreamConnectionTransport &_transport;

  public:
    DrainHandler(EventLoop &loop, StreamConnectionTransport &transport);
    ~DrainHandler();

    void onEvent();
  };

  static std::size_t const _chankSize;

  StreamConnectionProtocol &_protocol;
  ftpp::Socket _socket;
  Handler *_handler;
  DrainHandler *_drainHandler;
  std::vector<char> _buffer;
  bool _draining;
  bool _closed;

  StreamConnectionTransport();

public:
  StreamConnectionTransport(EventLoop &loop, StreamConnectionProtocol &protocol,
                            ftpp::Socket &socket);
  ~StreamConnectionTransport();

  void resume();
  void pause();
  void write(char const *data, size_t size);
  void drain();
  void close();
};

}; // namespace ftev