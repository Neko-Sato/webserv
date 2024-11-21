/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOWatcher.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 01:30:24 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/21 21:45:23 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop/BaseIOWatcher.hpp>

namespace ftev {

template <typename T> class IOWatcher : public EventLoop::BaseIOWatcher {
public:
  typedef void (*callback)(BaseIOWatcher &watcher, T data);

private:
  callback _on_read, _on_write, _on_except;
  T _data;

public:
  IOWatcher(EventLoop &loop, callback on_read, callback on_write,
            callback on_except, T data)
      : BaseIOWatcher(loop), _on_read(on_read), _on_write(on_write),
        _on_except(on_except), _data(data) {
  }

  void on_read() {
    if (_on_read)
      _on_read(*this, _data);
  }

  void on_write() {
    if (_on_write)
      _on_write(*this, _data);
  }

  void on_except() {
    if (_on_except)
      _on_except(*this, _data);
  }
};

} // namespace ftev
