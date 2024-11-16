/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTimerWatcher.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 05:31:53 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 03:19:48 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseTimerWatcher.hpp>

#include <cassert>

namespace ftev {

EventLoop::BaseTimerWatcher::BaseTimerWatcher(EventLoop &loop)
    : EventLoop::BaseWatcher(loop), _is_active(false) {
}

EventLoop::BaseTimerWatcher::~BaseTimerWatcher() {
  assert(!_is_active);
}

void EventLoop::BaseTimerWatcher::operator()() {
  assert(_is_active);
  _is_active = false;
  on_timeout();
}

void EventLoop::BaseTimerWatcher::start(time_t timeout) {
  assert(!_is_active);
  _is_active = true;
  loop._timer_watchers.insert(std::make_pair(loop._time + timeout, this));
}

} // namespace ftev
