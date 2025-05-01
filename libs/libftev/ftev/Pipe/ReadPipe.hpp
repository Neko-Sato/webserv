/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadPipe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:37:58 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/02 02:04:19 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/IOWatcher.hpp>

namespace ftev {

struct ReadPipeProtocol {
  virtual ~ReadPipeProtocol();
  virtual void onData(std::vector<char> const &data) = 0;
  virtual void onEof() = 0;
};

class ReadPipeTransport : private ftpp::NonCopyable {
private:
  class Handler : public EventLoop::IOWatcher {
  private:
    ReadPipeTransport &_transport;

  public:
    Handler(EventLoop &loop, ReadPipeTransport &transport);
    ~Handler();

    void onRead();
    void onWrite();
    void onExcept();
  };

  static std::size_t const _chankSize;

  ReadPipeProtocol &_protocol;
  int _fd;
  Handler *_handler;
  bool _closed;

  ReadPipeTransport();

public:
  ReadPipeTransport(EventLoop &loop, ReadPipeProtocol &protocol, int fd);
  ~ReadPipeTransport();

  void resume();
  void pause();
  void close();
};

} // namespace ftev
