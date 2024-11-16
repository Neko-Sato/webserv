/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTimerWatcher.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 05:31:53 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/16 17:08:10 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseTimerWatcher.hpp>
#include <cassert>

namespace ftev {

EventLoop::BaseTimerWatcher::BaseTimerWatcher(EventLoop &loop) : _loop(loop) {
}

EventLoop::BaseTimerWatcher::~BaseTimerWatcher() {
}

EventLoop::BaseTimerWatcher::BaseTimerWatcher(BaseTimerWatcher const &rhs)
    : _loop(rhs._loop) {
  assert(false);
}

EventLoop::BaseTimerWatcher &
EventLoop::BaseTimerWatcher::operator=(BaseTimerWatcher const &rhs) {
  (void)rhs;
  assert(false);
  return *this;
}

void EventLoop::BaseTimerWatcher::start(time_t timeout) {
  _loop._timer_watchers.insert(std::make_pair(_loop._time + timeout, this));
}

} // namespace ftev
