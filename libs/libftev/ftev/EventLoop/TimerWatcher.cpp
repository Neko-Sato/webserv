/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerWatcher.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 05:31:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/16 03:16:47 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/TimerWatcher.hpp>

#include <ftpp/algorithm.hpp>

#include <cassert>

namespace ftev {

EventLoop::TimerWatcher::TimerWatcher(EventLoop &loop)
    : loop(loop), _isActive(false) {
}

EventLoop::TimerWatcher::~TimerWatcher() {
  assert(!_isActive);
}

void EventLoop::TimerWatcher::start(time_t timeout) {
  assert(!_isActive);
  loop._updateTime();
  _it = loop._timerWatchers.insert(std::make_pair(loop._time + timeout, this));
  _isActive = true;
}

void EventLoop::TimerWatcher::cancel() {
  assert(_isActive);
  loop._timerWatchers.erase(_it);
  _isActive = false;
}

bool EventLoop::TimerWatcher::getIsActive() const {
  return _isActive;
}

} // namespace ftev
