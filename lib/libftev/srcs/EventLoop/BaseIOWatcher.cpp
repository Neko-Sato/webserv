/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseIOWatcher.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:48:55 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/20 04:47:11 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseIOWatcher.hpp>

#include <cassert>

namespace ftev {

EventLoop::BaseIOWatcher::BaseIOWatcher(EventLoop &loop)
    : EventLoop::BaseWatcher(loop) {
}

EventLoop::BaseIOWatcher::~BaseIOWatcher() {
  if (_is_active)
    stop();
  assert(!_is_active);
}

void EventLoop::BaseIOWatcher::operator()(event_details const &ev) {
  if (!_is_active)
    return;
  if (ev.events & ftpp::BaseSelector::READ)
    on_read();
  if (ev.events & ftpp::BaseSelector::WRITE)
    on_write();
  if (ev.events & ftpp::BaseSelector::ERROR)
    on_error();
}

void EventLoop::BaseIOWatcher::start(int fd, int events) {
  assert(!_is_active);
  loop._selector->add(fd, events);
  std::pair<IOWatchers::iterator, bool> result =
      loop._io_watchers.insert(std::make_pair(fd, this));
  assert(result.second);
  _it = result.first;
  _is_active = true;
}

void EventLoop::BaseIOWatcher::modify(int events) {
  assert(_is_active);
  loop._selector->modify(_it->first, events);
}

void EventLoop::BaseIOWatcher::stop() {
  assert(_is_active);
  loop._selector->remove(_it->first);
  loop._io_watchers.erase(_it);
  _is_active = false;
}

int EventLoop::BaseIOWatcher::get_fd() const {
  assert(_is_active);
  return _it->first;
}

} // namespace ftev
