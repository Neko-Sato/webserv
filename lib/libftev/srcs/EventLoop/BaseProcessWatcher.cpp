/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseProcessWatcher.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:23:58 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/16 17:10:39 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseProcessWatcher.hpp>
#include <EventLoop/WaitWatcher.hpp>
#include <cassert>

namespace ftev {

EventLoop::BaseProcessWatcher::BaseProcessWatcher(EventLoop &loop)
    : _loop(loop), _pid(-1) {
  if (!_loop._wait_watcher.get()) {
    _loop._wait_watcher.reset(new WaitWatcher(_loop));
    _loop._wait_watcher->start();
  }
}

EventLoop::BaseProcessWatcher::BaseProcessWatcher(BaseProcessWatcher const &rhs)
    : _loop(rhs._loop), _pid(rhs._pid) {
  assert(false);
}

EventLoop::BaseProcessWatcher &
EventLoop::BaseProcessWatcher::operator=(BaseProcessWatcher const &rhs) {
  (void)rhs;
  assert(false);
  return *this;
}

EventLoop::BaseProcessWatcher::~BaseProcessWatcher() {
}

void EventLoop::BaseProcessWatcher::start(pid_t pid) {
  assert(_pid == -1);
  _pid = pid;
  _loop._process_watchers[_pid] = this;
}

void EventLoop::BaseProcessWatcher::kill(int signum) {
  assert(_pid != -1);
  ::kill(_pid, signum);
}

} // namespace ftev
