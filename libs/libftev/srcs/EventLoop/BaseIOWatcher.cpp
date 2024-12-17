/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseIOWatcher.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:48:55 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/12 15:36:03 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseIOWatcher.hpp>

#include <cassert>

namespace ftev {

EventLoop::BaseIOWatcher::BaseIOWatcher(EventLoop &loop) : BaseWatcher(loop) {
}

EventLoop::BaseIOWatcher::~BaseIOWatcher() {
}

void EventLoop::BaseIOWatcher::operator()(event_t events) {
  if (_is_active && events & ftpp::BaseSelector::READ)
    on_read();
  if (_is_active && events & ftpp::BaseSelector::WRITE)
    on_write();
  if (_is_active && events & ftpp::BaseSelector::EXCEPT)
    on_except();
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

EventLoop::BaseIOWatcher::event_t EventLoop::BaseIOWatcher::get_events() const {
  assert(_is_active);
  return loop._selector->get_map().find(_it->first)->second;
}

} // namespace ftev
