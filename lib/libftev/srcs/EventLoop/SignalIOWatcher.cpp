/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalIOWatcher.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:19:02 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/16 17:40:03 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop/BaseSignalWatcher.hpp>
#include <EventLoop/SignalIOWatcher.hpp>
#include <cassert>

namespace ftev {

EventLoop::SignalIOWatcher::SignalIOWatcher(EventLoop &loop)
    : EventLoop::BaseIOWatcher(loop, EventLoop::_signal_pipe[0]) {
  assert(EventLoop::_signal_pipe[0] != -1);
}

EventLoop::SignalIOWatcher::~SignalIOWatcher() {
}

EventLoop::SignalIOWatcher::SignalIOWatcher(SignalIOWatcher const &rhs)
    : EventLoop::BaseIOWatcher(rhs._loop, rhs.get_fd()) {
  assert(false);
}

EventLoop::SignalIOWatcher &
EventLoop::SignalIOWatcher::operator=(SignalIOWatcher const &rhs) {
  (void)rhs;
  assert(false);
  return *this;
}

void EventLoop::SignalIOWatcher::on_read() {
  int signum;
  ssize_t size = read(get_fd(), &signum, sizeof(signum));
  assert(size == sizeof(signum));
  EventLoop::SignalWatchers::iterator it = _loop._signal_watchers.find(signum);
  if (it == _loop._signal_watchers.end())
	return;
  it->second->on_signal();
}

void EventLoop::SignalIOWatcher::on_write() {
  assert(false);
}

void EventLoop::SignalIOWatcher::on_error() {
  assert(false);
}

} // namespace ftev
