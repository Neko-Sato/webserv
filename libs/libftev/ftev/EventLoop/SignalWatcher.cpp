/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalWatcher.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 00:31:00 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/16 05:12:29 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/SignalWatcher.hpp>

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/macros.hpp>

#include <cassert>
#include <cstdlib>
#include <unistd.h>

namespace ftev {

EventLoop::SignalWatcher::SignalWatcher(EventLoop &loop)
    : loop(loop), _isActive(false) {
}

EventLoop::SignalWatcher::~SignalWatcher() {
  assert(!_isActive);
}

void EventLoop::SignalWatcher::start(int signum) {
  assert(!_isActive);
  SignalpipeWatcher::activate(loop);
  std::pair<SignalWatchers::iterator, bool> result =
      loop._signalWatchers.insert(std::make_pair(signum, this));
  assert(result.second);
  try {
    sighandler_t oldHandler = signal(signum, _signalHandler);
    if (unlikely(oldHandler == SIG_ERR))
      throw ftpp::OSError(errno, "signal");
    _oldHandler = oldHandler;
  } catch (...) {
    loop._signalWatchers.erase(signum);
    throw;
  }
  _it = result.first;
  _isActive = true;
}

void EventLoop::SignalWatcher::stop() {
  assert(_isActive);
  sighandler_t old_handler = signal(_it->first, _oldHandler);
  if (unlikely(old_handler == SIG_ERR))
    throw ftpp::OSError(errno, "signal");
  loop._signalWatchers.erase(_it);
  _isActive = false;
}

void EventLoop::SignalWatcher::_signalHandler(int signum) {
  assert(_signalpipe[1] != -1);
  ssize_t size = write(_signalpipe[1], &signum, sizeof(signum));
  UNUSED(size);
  assert(size == sizeof(signum));
}

EventLoop::SignalWatcher::SignalpipeWatcher::SignalpipeWatcher(EventLoop &loop)
    : IOWatcher(loop) {
}

EventLoop::SignalWatcher::SignalpipeWatcher::~SignalpipeWatcher() {
  if (getIsActive())
    stop();
}

void EventLoop::SignalWatcher::SignalpipeWatcher::activate(EventLoop &loop) {
  _maybeInitSignalpipe();
  if (unlikely(!loop._signalpipeWatcher))
    loop._signalpipeWatcher = new SignalpipeWatcher(loop);
  if (unlikely(!loop._signalpipeWatcher->getIsActive()))
    loop._signalpipeWatcher->start(EventLoop::_signalpipe[0],
                                   ftpp::Selector::READ);
}

void EventLoop::SignalWatcher::SignalpipeWatcher::onRead() {
  assert(_signalpipe[0] != -1);
  int signum;
  ssize_t size = read(_signalpipe[0], &signum, sizeof(signum));
  UNUSED(size);
  assert(size == sizeof(signum));
  SignalWatchers::iterator it = loop._signalWatchers.find(signum);
  if (it != loop._signalWatchers.end())
    it->second->onSignal();
}

void EventLoop::SignalWatcher::SignalpipeWatcher::onWrite() {
  assert(false);
}

void EventLoop::SignalWatcher::SignalpipeWatcher::onExcept() {
  assert(false);
}

bool EventLoop::SignalWatcher::getIsActive() const {
  return _isActive;
}

} // namespace ftev
