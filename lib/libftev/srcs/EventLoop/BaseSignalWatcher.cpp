/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSignalWatcher.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 00:31:00 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 20:45:50 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseSignalWatcher.hpp>
#include <Watchers/IOWatcher.hpp>
#include <exceptions/OSError.hpp>

#include <cassert>
#include <cstdlib>

namespace ftev {

EventLoop::BaseSignalWatcher::BaseSignalWatcher(EventLoop &loop)
    : EventLoop::BaseWatcher(loop), _is_active(false) {
}

EventLoop::BaseSignalWatcher::~BaseSignalWatcher() {
  assert(!_is_active);
}

bool EventLoop::BaseSignalWatcher::is_active() const {
  return _is_active;
}

void EventLoop::BaseSignalWatcher::operator()() {
  assert(_is_active);
  on_signal();
}

void EventLoop::BaseSignalWatcher::start(int signum) {
  assert(!_is_active);
  activate(loop);
  if (loop._old_sighandlers.find(signum) == loop._old_sighandlers.end()) {
    sighandler_t old_handler = signal(signum, _signal_handler);
    if (__glibc_unlikely(old_handler == SIG_ERR))
      throw ftpp::OSError(errno, "signal");
    loop._old_sighandlers[signum] = old_handler;
  }
  _it = loop._signal_watchers.insert(std::make_pair(signum, this));
  _is_active = true;
}

void EventLoop::BaseSignalWatcher::stop() {
  assert(_is_active);
  int signum = _it->first;
  if (loop._signal_watchers.count(signum) == 1) {
    sighandler_t old_handler = signal(signum, loop._old_sighandlers[signum]);
    if (__glibc_unlikely(old_handler == SIG_ERR))
      throw ftpp::OSError(errno, "signal");
    loop._old_sighandlers.erase(signum);
  }
  loop._signal_watchers.erase(_it);
  _is_active = false;
}

void EventLoop::BaseSignalWatcher::_signal_handler(int signum) {
  ssize_t size = write(_pipe[1], &signum, sizeof(signum));
  assert(size == sizeof(signum));
}

void EventLoop::BaseSignalWatcher::_signal_pipe_on_read(BaseIOWatcher &watcher,
                                                        int _) {
  (void)_;
  int signum;
  ssize_t size = read(watcher.get_fd(), &signum, sizeof(signum));
  assert(size == sizeof(signum));
  std::pair<SignalWatchers::iterator, SignalWatchers::iterator> range =
      watcher.loop._signal_watchers.equal_range(signum);
  for (SignalWatchers::iterator it = range.first; it != range.second; ++it)
    it->second->operator()();
}

int EventLoop::BaseSignalWatcher::_pipe[2] = {-1, -1};

void EventLoop::BaseSignalWatcher::_signal_pipe_maybe_acquire() {
  if (__glibc_unlikely(_pipe[0] == -1 || _pipe[1] == -1)) {
    if (__glibc_unlikely(pipe(_pipe) == -1))
      throw ftpp::OSError(errno, "pipe");
    std::atexit(_signal_pipe_release);
  }
}

void EventLoop::BaseSignalWatcher::_signal_pipe_release() {
  close(_pipe[0]), close(_pipe[1]);
  _pipe[0] = _pipe[1] = -1;
}

void EventLoop::BaseSignalWatcher::activate(EventLoop &loop) {
  _signal_pipe_maybe_acquire();
  if (__glibc_unlikely(!loop._signal_io_watcher.get()))
    loop._signal_io_watcher.reset(
        new IOWatcher<int>(loop, _signal_pipe_on_read, NULL, NULL, 0));
  if (__glibc_unlikely(!loop._signal_io_watcher->is_active()))
    loop._signal_io_watcher->start(_pipe[0], ftpp::BaseSelector::READ);
}

} // namespace ftev
