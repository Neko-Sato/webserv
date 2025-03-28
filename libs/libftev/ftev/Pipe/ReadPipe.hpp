/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadPipe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:37:58 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/28 22:17:18 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/IOWatcher.hpp>

namespace ftev {

class ReadPipe : private EventLoop::IOWatcher {
private:
  static std::size_t const _chank_size;
  int _fd;
  bool _received_eof;

public:
  ReadPipe(EventLoop &loop, int fd);
  virtual ~ReadPipe();

  using IOWatcher::loop;

  void on_read();
  void on_write();

  void resume();
  void pause();

  virtual void on_data(std::vector<char> const &data) = 0;
  virtual void on_eof() = 0;
};

} // namespace ftev
