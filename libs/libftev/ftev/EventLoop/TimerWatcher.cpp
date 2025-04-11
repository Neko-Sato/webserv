/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerWatcher.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 05:31:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/11 18:15:52 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/TimerWatcher.hpp>

#include <cassert>

namespace ftev {

EventLoop::TimerWatcher::TimerWatcher(EventLoop &loop)
    : Watcher(loop), _is_active(false) {
}

EventLoop::TimerWatcher::~TimerWatcher() {
  assert(!_is_active);
}

void EventLoop::TimerWatcher::operator()() {
  if (!_is_active)
    return;
  loop._timer_watchers.erase(_it);
  _is_active = false;
  on_timeout();
}

void EventLoop::TimerWatcher::start(time_t timeout) {
  assert(!_is_active);
  _is_active = true;
  loop._update_time();
  _it = loop._timer_watchers.insert(std::make_pair(loop._time + timeout, this));
}

void EventLoop::TimerWatcher::cancel() {
  assert(_is_active);
  loop._timer_watchers.erase(_it);
  _is_active = false;
}

bool EventLoop::TimerWatcher::is_active() const {
  return _is_active;
}

} // namespace ftev
