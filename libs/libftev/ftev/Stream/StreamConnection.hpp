/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StreamConnection.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:50:15 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/28 22:44:00 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/IOWatcher.hpp>

#include <ftpp/socket/Socket.hpp>

namespace ftev {

class StreamConnection : private EventLoop::IOWatcher {
private:
  static std::size_t const _chank_size;
  ftpp::Socket _socket;
  bool _received_eof;
  std::vector<char> _buffer;
  bool _draining;

protected:
  StreamConnection(EventLoop &loop, ftpp::Socket &socket);

public:
  virtual ~StreamConnection();

  using IOWatcher::loop;

  void on_read();
  void on_write();

  void resume();
  void pause();
  void write(char const *data, size_t size);
  void drain();

  virtual void on_data(std::vector<char> const &data) = 0;
  virtual void on_eof() = 0;
  virtual void on_drain() = 0;
};

}; // namespace ftev