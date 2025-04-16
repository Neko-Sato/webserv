/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOWatcher.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:48:55 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/16 05:03:51 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/IOWatcher.hpp>

#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>

#include <cassert>

namespace ftev {

EventLoop::IOWatcher::IOWatcher(EventLoop &loop)
    : loop(loop), _isActive(false) {
}

EventLoop::IOWatcher::~IOWatcher() {
  assert(!_isActive);
}

void EventLoop::IOWatcher::start(int fd, event_t events) {
  assert(!_isActive);
  std::pair<IOWatchers::iterator, bool> result =
      loop._ioWatchers.insert(std::make_pair(fd, this));
  assert(result.second);
  try {
    loop._selector->add(fd, events);
  } catch (...) {
    loop._ioWatchers.erase(fd);
    throw;
  }
  _it = result.first;
  _isActive = true;
}

void EventLoop::IOWatcher::modify(event_t events) {
  assert(_isActive);
  loop._selector->modify(_it->first, events);
}

void EventLoop::IOWatcher::stop() {
  assert(_isActive);
  loop._selector->remove(_it->first);
  loop._ioWatchers.erase(_it);
  _isActive = false;
}

EventLoop::IOWatcher::event_t EventLoop::IOWatcher::getEvents() const {
  assert(_isActive);
  return loop._selector->getMap().find(_it->first)->second;
}

bool EventLoop::IOWatcher::getIsActive() const {
  return _isActive;
}

} // namespace ftev
