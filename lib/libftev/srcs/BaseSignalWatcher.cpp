/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSignalWatcher.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 00:31:00 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/16 15:18:58 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <BaseSignalWatcher.hpp>
#include <EventLoop.hpp>
#include <SignalIOWatcher.hpp>
#include <cassert>
#include <cstdlib>
#include <exceptions/OSError.hpp>

namespace ftev {

EventLoop::BaseSignalWatcher::BaseSignalWatcher(EventLoop &loop)
    : _loop(loop), _signum(-1), _old_handler(SIG_DFL) {
  _loop._acquire_signal_pipe();
  if (!_loop._signal_io_watcher.get()) {
    _loop._signal_io_watcher.reset(new SignalIOWatcher(_loop));
    _loop._signal_io_watcher->start(EventLoop::_signal_pipe[0]);
  }
}

EventLoop::BaseSignalWatcher::~BaseSignalWatcher() {
}

EventLoop::BaseSignalWatcher::BaseSignalWatcher(BaseSignalWatcher const &rhs)
    : _loop(rhs._loop), _signum(rhs._signum), _old_handler(rhs._old_handler) {
  assert(false);
}

EventLoop::BaseSignalWatcher &
EventLoop::BaseSignalWatcher::operator=(BaseSignalWatcher const &rhs) {
  (void)rhs;
  assert(false);
  return *this;
}

void EventLoop::BaseSignalWatcher::operator()() {
  on_signal();
}

void EventLoop::BaseSignalWatcher::_signal_handler(int signum) {
  assert(EventLoop::_signal_pipe[1] != -1);
  ssize_t size = write(EventLoop::_signal_pipe[1], &signum, sizeof(signum));
  (void)size;
}

void EventLoop::BaseSignalWatcher::start(int signum) {
  _signum = signum;
  _old_handler = signal(_signum, _signal_handler);
  if (__glibc_unlikely(_old_handler == SIG_ERR))
    throw ftpp::OSError(errno, "signal");
  _loop._signal_watchers[_signum] = this;
}

void EventLoop::BaseSignalWatcher::stop() {
  sighandler_t tmp = signal(_signum, _old_handler);
  if (__glibc_unlikely(tmp == SIG_ERR))
    throw ftpp::OSError(errno, "signal");
  _loop._signal_watchers.erase(_signum);
}

} // namespace ftev
