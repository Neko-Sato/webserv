/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOWatcher.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 01:30:24 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 02:35:10 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop/BaseIOWatcher.hpp>

namespace ftev {

template <typename T> class IOWatcher : public EventLoop::BaseIOWatcher {
public:
  typedef void (*callback)(IOWatcher &watcher, T data);

private:
  callback _on_read, _on_write, _on_error;
  T _data;

public:
  IOWatcher(EventLoop &loop, callback on_read, callback on_write,
            callback on_error, T data)
      : BaseIOWatcher(loop), _on_read(on_read), _on_write(on_write),
        _on_error(on_error), _data(data) {
  }

  void on_read() {
    if (_on_read)
      _on_read(*this, _data);
  }

  void on_write() {
    if (_on_write)
      _on_write(*this, _data);
  }

  void on_error() {
    if (_on_error)
      _on_error(*this, _data);
  }
};

} // namespace ftev
