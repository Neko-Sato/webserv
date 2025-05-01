/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WritePipe.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:45:51 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/02 02:37:18 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/DeferWatcher.hpp>
#include <ftev/EventLoop/IOWatcher.hpp>

namespace ftev {

struct WritePipeProtocol {
  virtual ~WritePipeProtocol();
  virtual void onDrain() = 0;
};

class WritePipeTransport : private ftpp::NonCopyable {
private:
  class Handler : public EventLoop::IOWatcher {
  private:
    WritePipeTransport &_transport;

  public:
    Handler(EventLoop &loop, WritePipeTransport &transport);
    ~Handler();

    void onRead();
    void onWrite();
    void onExcept();
  };

  class DrainHandler : public EventLoop::DeferWatcher {
  private:
    WritePipeTransport &_transport;

  public:
    DrainHandler(EventLoop &loop, WritePipeTransport &transport);
    ~DrainHandler();

    void onEvent();
  };

  WritePipeProtocol &_protocol;
  int _fd;
  Handler *_handler;
  DrainHandler *_drainHandler;
  bool _closed;
  std::vector<char> _buffer;
  bool _draining;

  WritePipeTransport();

public:
  WritePipeTransport(EventLoop &loop, WritePipeProtocol &protocol, int fd);
  ~WritePipeTransport();

  void write(char const *data, size_t size);
  void drain();
  void close();
};

}; // namespace ftev