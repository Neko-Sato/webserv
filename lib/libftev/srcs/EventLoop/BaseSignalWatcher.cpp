/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSignalWatcher.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 00:31:00 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 07:50:22 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseSignalWatcher.hpp>
#include <EventLoop/SignalPipe.hpp>
#include <Watchers/IOWatcher.hpp>
#include <exceptions/OSError.hpp>

#include <cassert>
#include <cstdlib>

namespace ftev {

EventLoop::BaseSignalWatcher::BaseSignalWatcher(EventLoop &loop)
    : EventLoop::BaseWatcher(loop), _is_active(false), _signum(-1),
      _is_current(false), _old_handler(SIG_DFL), _old_watcher(NULL) {
  if (__glibc_unlikely(!loop._signal_pipe.get()))
    loop._signal_pipe.reset(new SignalPipe);
  if (__glibc_unlikely(!loop._signal_io_watcher.get())) {
    loop._signal_io_watcher.reset(
        new IOWatcher<int>(loop, _signal_pipe_on_read, NULL, NULL, 0));
    loop._signal_io_watcher->start(EventLoop::_signal_pipe->get_in(),
                                   ftpp::BaseSelector::READ);
  }
}

EventLoop::BaseSignalWatcher::~BaseSignalWatcher() {
  if (_is_active && _is_current)
    stop();
  assert(!_is_active);
}

void EventLoop::BaseSignalWatcher::operator()() {
  assert(_is_active);
  on_signal();
}

void EventLoop::BaseSignalWatcher::start(int signum) {
  assert(!_is_active);
  _signum = signum;
  _old_handler = signal(_signum, _signal_handler);
  if (__glibc_unlikely(_old_handler == SIG_ERR))
    throw ftpp::OSError(errno, "signal");
  _old_watcher = loop._signal_watchers[_signum];
  loop._signal_watchers[_signum] = this;
  if (_old_watcher)
    _old_watcher->_is_current = false;
  _is_current = true;
  _is_active = true;
}

void EventLoop::BaseSignalWatcher::stop() {
  assert(_is_active);
  assert(_is_current);
  sighandler_t tmp = signal(_signum, _old_handler);
  if (__glibc_unlikely(tmp == SIG_ERR))
    throw ftpp::OSError(errno, "signal");
  if (_old_watcher) {
    loop._signal_watchers[_signum] = _old_watcher;
    _old_watcher->_is_current = true;
  } else
    loop._signal_watchers.erase(_signum);
  _is_active = false;
}

void EventLoop::BaseSignalWatcher::_signal_handler(int signum) {
  ssize_t size =
      write(EventLoop::_signal_pipe->get_out(), &signum, sizeof(signum));
  assert(size == sizeof(signum));
}

void EventLoop::BaseSignalWatcher::_signal_pipe_on_read(IOWatcher<int> &watcher,
                                                        int _) {
  (void)_;
  int signum;
  ssize_t size = read(watcher.get_fd(), &signum, sizeof(signum));
  assert(size == sizeof(signum));
  EventLoop::SignalWatchers::iterator it =
      watcher.loop._signal_watchers.find(signum);
  if (it != watcher.loop._signal_watchers.end())
    it->second->operator()();
}

} // namespace ftev
