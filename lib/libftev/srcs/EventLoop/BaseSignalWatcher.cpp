/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSignalWatcher.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 00:31:00 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/20 03:42:02 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseSignalWatcher.hpp>
#include <Watchers/IOWatcher.hpp>
#include <exceptions/OSError.hpp>

#include <cassert>
#include <cstdlib>
#include <fcntl.h>

namespace ftev {

int EventLoop::BaseSignalWatcher::_pipe[2] = {-1, -1};

EventLoop::BaseSignalWatcher::BaseSignalWatcher(EventLoop &loop)
    : EventLoop::BaseWatcher(loop) {
}

EventLoop::BaseSignalWatcher::~BaseSignalWatcher() {
  if (_is_active)
    stop();
  assert(!_is_active);
}

void EventLoop::BaseSignalWatcher::operator()() {
  if (!_is_active)
    return;
  on_signal();
}

void EventLoop::BaseSignalWatcher::start(int signum) {
  assert(!_is_active);
  _activate();
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
  for (SignalWatchers::iterator it = watcher.loop._signal_watchers.find(signum);
       it != watcher.loop._signal_watchers.end() && it->first == signum; ++it)
    it->second->operator()();
}

void EventLoop::BaseSignalWatcher::_activate() {
  if (__glibc_unlikely(_pipe[0] == -1 || _pipe[1] == -1)) {
    if (__glibc_unlikely(pipe(_pipe) == -1))
      throw ftpp::OSError(errno, "pipe");
    int flags;
    flags = fcntl(_pipe[0], F_GETFD);
    if (__glibc_unlikely(flags == -1))
      throw ftpp::OSError(errno, "fcntl");
    if (__glibc_unlikely(fcntl(_pipe[0], F_SETFD, flags | FD_CLOEXEC) == -1))
      throw ftpp::OSError(errno, "fcntl");
    flags = fcntl(_pipe[1], F_GETFD);
    if (__glibc_unlikely(flags == -1))
      throw ftpp::OSError(errno, "fcntl");
    if (__glibc_unlikely(fcntl(_pipe[1], F_SETFD, flags | FD_CLOEXEC) == -1))
      throw ftpp::OSError(errno, "fcntl");
    if (__glibc_unlikely(std::atexit(_pipe_release) == -1))
      throw ftpp::OSError(errno, "atexit");
  }
  if (__glibc_unlikely(!loop._signalpipe_watcher))
    loop._signalpipe_watcher =
        new IOWatcher<int>(loop, _signal_pipe_on_read, NULL, NULL, 0);
  if (__glibc_unlikely(!loop._signalpipe_watcher->is_active()))
    loop._signalpipe_watcher->start(_pipe[0], ftpp::BaseSelector::READ);
}

void EventLoop::BaseSignalWatcher::_pipe_release() {
  close(_pipe[0]), close(_pipe[1]);
  _pipe[0] = _pipe[1] = -1;
}

} // namespace ftev
