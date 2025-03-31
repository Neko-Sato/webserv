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
#include <ftev/EventLoop/IOWatcher.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>
#include <ftpp/socket/Socket.hpp>

namespace ftev {

struct StreamConnectionProtocol {
  virtual void on_data(std::vector<char> const &data) = 0;
  virtual void on_eof() = 0;
  virtual void on_drain() = 0;
  virtual void on_except() = 0;
};

class StreamConnectionTransport : private ftpp::NonCopyable {
private:
  class Handler : public EventLoop::IOWatcher {
  private:
    StreamConnectionTransport &_transport;

  public:
    Handler(EventLoop &loop, StreamConnectionTransport &transport);
    ~Handler();

    void on_read();
    void on_write();
    void on_except();
  };

  static std::size_t const _chank_size;

  StreamConnectionProtocol &_protocol;
  ftpp::Socket _socket;
  Handler *_handler;
  std::vector<char> _buffer;
  bool _draining;

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