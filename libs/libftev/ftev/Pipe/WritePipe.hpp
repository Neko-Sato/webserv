/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WritePipe.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:45:51 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/02 00:19:58 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/IOWatcher.hpp>

namespace ftev {

struct WritePipeProtocol {
  virtual void on_drain() = 0;
  virtual void on_except() = 0;
};

class WritePipeTransport : private ftpp::NonCopyable {
private:
  class Handler : public EventLoop::IOWatcher {
  private:
    WritePipeTransport &_transport;

  public:
    Handler(EventLoop &loop, WritePipeTransport &transport);
    ~Handler();

    void on_read();
    void on_write();
    void on_except();
  };

  WritePipeProtocol &_protocol;
  int _fd;
  Handler *_handler;
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