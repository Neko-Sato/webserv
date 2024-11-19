/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProcessWatcher.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 01:30:24 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/20 00:46:24 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop/BaseProcessWatcher.hpp>

namespace ftev {

template <typename T>
class ProcessWatcher : public EventLoop::BaseProcessWatcher {
public:
  typedef void (*callback)(BaseProcessWatcher &watcher, int status, T data);

private:
  callback _on_exited;
  callback _on_signaled;
  T _data;

public:
  ProcessWatcher(EventLoop &loop, callback on_exited, callback on_signaled,
                 T data)
      : BaseProcessWatcher(loop), _on_exited(on_exited),
        _on_signaled(on_signaled), _data(data) {
  }

  void on_exited(int status) {
    if (_on_exited)
      _on_exited(*this, status, _data);
  }

  void on_signaled(int signum) {
    if (_on_signaled)
      _on_signaled(*this, signum, _data);
  }
};

} // namespace ftev
