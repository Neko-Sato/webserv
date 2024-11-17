/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseIOWatcher.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:48:55 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 20:53:51 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseIOWatcher.hpp>

#include <cassert>

namespace ftev {

EventLoop::BaseIOWatcher::BaseIOWatcher(EventLoop &loop)
    : EventLoop::BaseWatcher(loop), _is_active(false) {
}

EventLoop::BaseIOWatcher::~BaseIOWatcher() {
  assert(!_is_active);
}

bool EventLoop::BaseIOWatcher::is_active() const {
  return _is_active;
}

void EventLoop::BaseIOWatcher::operator()(event_detals const &ev) {
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
  _it = loop._io_watchers.insert(std::make_pair(fd, this)).first;
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
