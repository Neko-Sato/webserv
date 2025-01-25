/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSignalWatcher.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 00:31:00 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/25 08:03:50 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseSignalWatcher.hpp>

#include <exceptions/OSError.hpp>
#include <macros.hpp>

#include <cassert>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

namespace ftev {

EventLoop::BaseSignalWatcher::BaseSignalWatcher(EventLoop &loop)
    : BaseWatcher(loop) {
}

EventLoop::BaseSignalWatcher::~BaseSignalWatcher() {
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
    if (unlikely(old_handler == SIG_ERR))
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
  if (unlikely(old_handler == SIG_ERR))
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
  if (is_active())
    stop();
}

void EventLoop::BaseSignalWatcher::SignalpipeWatcher::on_read() {
  assert(_signalpipe[0] != -1);
  int signum;
  ssize_t size = read(_signalpipe[0], &signum, sizeof(signum));
  assert(size == sizeof(signum));
  SignalWatchers::iterator it = loop._signal_watchers.find(signum);
  if (it != loop._signal_watchers.end())
    (*it->second)();
}

void EventLoop::BaseSignalWatcher::SignalpipeWatcher::on_write() {
  assert(false);
}

void EventLoop::BaseSignalWatcher::SignalpipeWatcher::on_except() {
  assert(false);
}

void EventLoop::BaseSignalWatcher::_activate() {
  EventLoop::_maybe_init_signalpipe();
  if (unlikely(!loop._signalpipe_watcher))
    loop._signalpipe_watcher = new SignalpipeWatcher(loop);
  if (unlikely(!loop._signalpipe_watcher->is_active()))
    loop._signalpipe_watcher->start(EventLoop::_signalpipe[0],
                                    ftpp::BaseSelector::READ);
}

} // namespace ftev
