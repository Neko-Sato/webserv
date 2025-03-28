/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOWatcher.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:48:55 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/28 22:15:01 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/IOWatcher.hpp>

#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>

#include <cassert>

namespace ftev {

EventLoop::IOWatcher::IOWatcher(EventLoop &loop)
    : Watcher(loop), _is_active(false) {
}

EventLoop::IOWatcher::~IOWatcher() {
  assert(!_is_active);
}

void EventLoop::IOWatcher::operator()(event_t events) {
  if (_is_active && events & ftpp::Selector::EXCEPT) {
    on_except();
    return;
  }
  if (_is_active && events & ftpp::Selector::READ)
    on_read();
  if (_is_active && events & ftpp::Selector::WRITE)
    on_write();
}

EventLoop::IOWatcher::event_t EventLoop::IOWatcher::get_events() const {
  assert(_is_active);
  return loop._selector->getMap().find(_it->first)->second;
}

void EventLoop::IOWatcher::start(int fd, event_t events) {
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

void EventLoop::IOWatcher::modify(event_t events) {
  assert(_is_active);
  loop._selector->modify(_it->first, events);
}

void EventLoop::IOWatcher::stop() {
  assert(_is_active);
  loop._selector->remove(_it->first);
  loop._io_watchers.erase(_it);
  _is_active = false;
}

bool EventLoop::IOWatcher::is_active() const {
  return _is_active;
}

} // namespace ftev
