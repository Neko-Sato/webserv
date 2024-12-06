/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTimerWatcher.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 05:31:53 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/26 13:37:08 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseTimerWatcher.hpp>

#include <cassert>

namespace ftev {

EventLoop::BaseTimerWatcher::BaseTimerWatcher(EventLoop &loop)
    : BaseWatcher(loop) {
}

EventLoop::BaseTimerWatcher::~BaseTimerWatcher() {
  if (_is_active)
    cancel();
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

} // namespace ftev
