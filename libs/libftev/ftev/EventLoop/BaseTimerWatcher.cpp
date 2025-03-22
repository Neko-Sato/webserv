/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTimerWatcher.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 05:31:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/21 03:24:10 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/BaseTimerWatcher.hpp>

#include <cassert>

namespace ftev {

EventLoop::BaseTimerWatcher::BaseTimerWatcher(EventLoop &loop)
    : BaseWatcher(loop), _is_active(false) {
}

EventLoop::BaseTimerWatcher::~BaseTimerWatcher() {
  assert(!_is_active);
}

void EventLoop::BaseTimerWatcher::operator()() {
  if (!_is_active)
    return;
  loop._timer_watchers.erase(_it);
  _is_active = false;
  on_timeout();
}

void EventLoop::BaseTimerWatcher::start(time_t timeout) {
  assert(!_is_active);
  _is_active = true;
  _it = loop._timer_watchers.insert(std::make_pair(loop._time + timeout, this));
}

void EventLoop::BaseTimerWatcher::cancel() {
  assert(_is_active);
  loop._timer_watchers.erase(_it);
  _is_active = false;
}

bool EventLoop::BaseTimerWatcher::is_active() const {
  return _is_active;
}

} // namespace ftev
