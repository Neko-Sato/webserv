/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WritePipe.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:45:51 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/28 22:17:18 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/IOWatcher.hpp>

namespace ftev {

class WritePipe : private EventLoop::IOWatcher {
private:
  int _fd;
  std::vector<char> _buffer;
  bool _draining;

public:
  WritePipe(EventLoop &loop, int fd);
  virtual ~WritePipe();

  using IOWatcher::loop;

  void on_read();
  void on_write();

  void write(char const *data, size_t size);
  void drain();

  virtual void on_drain() = 0;
};

}; // namespace ftev