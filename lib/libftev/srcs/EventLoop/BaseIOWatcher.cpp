/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseIOWatcher.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:48:55 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 07:41:15 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseIOWatcher.hpp>

#include <cassert>

namespace ftev {

EventLoop::BaseIOWatcher::BaseIOWatcher(EventLoop &loop)
    : EventLoop::BaseWatcher(loop), _is_active(false), _fd(-1) {
}

EventLoop::BaseIOWatcher::~BaseIOWatcher() {
  assert(!_is_active);
}

void EventLoop::BaseIOWatcher::operator()(event_detals const &ev) {
  assert(_is_active);
  if (ev.events & ftpp::BaseSelector::READ)
    on_read();
  if (ev.events & ftpp::BaseSelector::WRITE)
    on_write();
  if (ev.events & ftpp::BaseSelector::ERROR)
    on_error();
}

void EventLoop::BaseIOWatcher::start(int fd, int events) {
  assert(!_is_active);
  _fd = fd;
  loop._selector->add(_fd, events);
  loop._io_watchers[_fd] = this;
  _is_active = true;
}

void EventLoop::BaseIOWatcher::modify(int events) {
  assert(_is_active);
  loop._selector->modify(_fd, events);
}

void EventLoop::BaseIOWatcher::stop() {
  assert(_is_active);
  loop._selector->remove(_fd);
  loop._io_watchers.erase(_fd);
  _is_active = false;
}

int EventLoop::BaseIOWatcher::get_fd() const {
  return _fd;
}

} // namespace ftev
