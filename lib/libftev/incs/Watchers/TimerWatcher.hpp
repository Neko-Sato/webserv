/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerWatcher.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 01:30:24 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/20 00:47:01 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop/BaseTimerWatcher.hpp>

namespace ftev {

template <typename T> class TimerWatcher : public EventLoop::BaseTimerWatcher {
public:
  typedef void (*callback)(BaseTimerWatcher &watcher, T data);

private:
  callback _on_timeout;
  T _data;

public:
  TimerWatcher(EventLoop &loop, callback on_timeout, T data)
      : BaseTimerWatcher(loop), _on_timeout(on_timeout), _data(data) {
  }

  void on_timeout() {
    if (_on_timeout)
      _on_timeout(*this, _data);
  }
};

} // namespace ftev
