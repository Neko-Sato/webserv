/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseIOWatcher.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:48:55 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/22 15:18:18 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/BaseIOWatcher.hpp>

#include <cassert>

namespace ftev {

EventLoop::BaseIOWatcher::BaseIOWatcher(EventLoop &loop)
    : BaseWatcher(loop), _is_active(false) {
}

EventLoop::BaseIOWatcher::~BaseIOWatcher() {
  assert(!_is_active);
}

void EventLoop::BaseIOWatcher::operator()(event_t events) {
  if (_is_active && events & ftpp::BaseSelector::EXCEPT) {
    on_except();
    return;
  }
  if (_is_active && events & ftpp::BaseSelector::READ)
    on_read();
  if (_is_active && events & ftpp::BaseSelector::WRITE)
    on_write();
}

EventLoop::BaseIOWatcher::event_t EventLoop::BaseIOWatcher::get_events() const {
  assert(_is_active);
  return loop._selector->getMap().find(_it->first)->second;
}

void EventLoop::BaseIOWatcher::start(int fd, event_t events) {
  assert(!_is_active);
  std::pair<IOWatchers::iterator, bool> result =
      loop._io_watchers.insert(std::make_pair(fd, this));
  assert(result.second);
  _it = result.first;
  try {
    loop._selector->add(fd, events);
  } catch (...) {
    loop._io_watchers.erase(_it);
    throw;
  }
  _is_active = true;
}

void EventLoop::BaseIOWatcher::modify(event_t events) {
  assert(_is_active);
  loop._selector->modify(_it->first, events);
}

void EventLoop::BaseIOWatcher::stop() {
  assert(_is_active);
  loop._selector->remove(_it->first);
  loop._io_watchers.erase(_it);
  _is_active = false;
}

bool EventLoop::BaseIOWatcher::is_active() const {
  return _is_active;
}

} // namespace ftev
