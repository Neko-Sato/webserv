/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSignalWatcher.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 00:31:00 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/26 13:37:03 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseSignalWatcher.hpp>
#include <exceptions/OSError.hpp>

#include <cassert>
#include <cstdlib>
#include <fcntl.h>

namespace ftev {

EventLoop::BaseSignalWatcher::BaseSignalWatcher(EventLoop &loop)
    : BaseWatcher(loop) {
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
  std::pair<SignalWatchers::iterator, bool> result =
      loop._signal_watchers.insert(std::make_pair(signum, this));
  assert(result.second);
  _it = result.first;
  try {
    sighandler_t old_handler = signal(signum, _signal_handler);
    if (__glibc_unlikely(old_handler == SIG_ERR))
      throw ftpp::OSError(errno, "signal");
    _old_handler = old_handler;
  } catch (...) {
    loop._signal_watchers.erase(_it);
    throw;
  }
  _is_active = true;
}

void EventLoop::BaseSignalWatcher::stop() {
  assert(_is_active);
  sighandler_t old_handler = signal(_it->first, _old_handler);
  if (__glibc_unlikely(old_handler == SIG_ERR))
    throw ftpp::OSError(errno, "signal");
  loop._signal_watchers.erase(_it);
  _is_active = false;
}

void EventLoop::BaseSignalWatcher::_signal_handler(int signum) {
  assert(_signalpipe[1] != -1);
  ssize_t size = write(_signalpipe[1], &signum, sizeof(signum));
  assert(size == sizeof(signum));
}

EventLoop::BaseSignalWatcher::SignalpipeWatcher::SignalpipeWatcher(
    EventLoop &loop)
    : BaseIOWatcher(loop) {
}

EventLoop::BaseSignalWatcher::SignalpipeWatcher::~SignalpipeWatcher() {
}

void EventLoop::BaseSignalWatcher::SignalpipeWatcher::on_read() {
  assert(_signalpipe[0] != -1);
  int signum;
  ssize_t size = read(_signalpipe[0], &signum, sizeof(signum));
  assert(size == sizeof(signum));
  SignalWatchers::iterator it = loop._signal_watchers.find(signum);
  if (it != loop._signal_watchers.end())
    it->second->operator()();
}

void EventLoop::BaseSignalWatcher::SignalpipeWatcher::on_write() {
  assert(false);
}

void EventLoop::BaseSignalWatcher::SignalpipeWatcher::on_except() {
  assert(false);
}

void EventLoop::BaseSignalWatcher::_activate() {
  int &pipe_in = _signalpipe[0];
  int &pipe_out = _signalpipe[1];
  if (__glibc_unlikely(pipe_in == -1 || pipe_out == -1)) {
    if (__glibc_unlikely(pipe(_signalpipe) == -1))
      throw ftpp::OSError(errno, "pipe");
    int flags;
    flags = fcntl(pipe_in, F_GETFD);
    if (__glibc_unlikely(flags == -1))
      throw ftpp::OSError(errno, "fcntl");
    if (__glibc_unlikely(fcntl(pipe_in, F_SETFD, flags | FD_CLOEXEC) == -1))
      throw ftpp::OSError(errno, "fcntl");
    flags = fcntl(pipe_out, F_GETFD);
    if (__glibc_unlikely(flags == -1))
      throw ftpp::OSError(errno, "fcntl");
    if (__glibc_unlikely(fcntl(pipe_out, F_SETFD, flags | FD_CLOEXEC) == -1))
      throw ftpp::OSError(errno, "fcntl");
  }
  if (__glibc_unlikely(!loop._signalpipe_watcher))
    loop._signalpipe_watcher = new SignalpipeWatcher(loop);
  if (__glibc_unlikely(!loop._signalpipe_watcher->is_active()))
    loop._signalpipe_watcher->start(pipe_in, ftpp::BaseSelector::READ);
}

} // namespace ftev
