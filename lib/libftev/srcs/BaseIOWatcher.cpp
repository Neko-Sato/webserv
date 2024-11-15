/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseIOWatcher.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:48:55 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/16 00:19:36 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <BaseIOWatcher.hpp>
#include <EventLoop.hpp>
#include <cassert>
#include <unistd.h>

namespace ftev {

EventLoop::BaseIOWatcher::BaseIOWatcher(EventLoop &loop, int fd)
    : _loop(loop), _fd(fd) {
}

EventLoop::BaseIOWatcher::~BaseIOWatcher() {
}

EventLoop::BaseIOWatcher::BaseIOWatcher(EventLoop::BaseIOWatcher const &rhs)
    : _loop(rhs._loop), _fd(rhs._fd) {
  assert(false);
}

EventLoop::BaseIOWatcher &
EventLoop::BaseIOWatcher::operator=(EventLoop::BaseIOWatcher const &rhs) {
  (void)rhs;
  assert(false);
  return *this;
}

void EventLoop::BaseIOWatcher::operator()(event_detals const &ev) {
  if (ev.events & ftpp::BaseSelector::READ)
    on_read();
  if (ev.events & ftpp::BaseSelector::WRITE)
    on_write();
  if (ev.events & ftpp::BaseSelector::ERROR)
    on_error();
}

void EventLoop::BaseIOWatcher::start(int events) {
  _loop._selector->add(_fd, events);
  _loop._io_watchers[_fd] = this;
}

void EventLoop::BaseIOWatcher::modify(int events) {
  _loop._selector->modify(_fd, events);
}

void EventLoop::BaseIOWatcher::close() {
  _loop._selector->remove(_fd);
  _loop._io_watchers.erase(_fd);
  ::close(_fd);
  _fd = -1;
}

int EventLoop::BaseIOWatcher::get_fd() const {
  return _fd;
}

} // namespace ftev
