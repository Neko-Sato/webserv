/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:57:51 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/26 01:26:12 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/DeferWatcher.hpp>
#include <ftev/EventLoop/IOWatcher.hpp>
#include <ftev/EventLoop/ProcessWatcher.hpp>
#include <ftev/EventLoop/Reaper.hpp>
#include <ftev/EventLoop/SignalWatcher.hpp>
#include <ftev/EventLoop/TimerWatcher.hpp>

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/macros.hpp>
#include <ftpp/selectors/Selector.hpp>

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fcntl.h>
#include <functional>
#include <limits>
#include <unistd.h>

namespace ftev {

int EventLoop::_signalpipe[2] = {-1, -1};

ftpp::Selector *EventLoop::defaultSelectorFactory() {
  return new ftpp::DefaultSelector;
}

EventLoop::EventLoop(selector_factory_t factory)
    : _selector(factory()), _time(0), _running(false), _stopFlag(false),
      _signalpipeWatcher(NULL), _waitWatcher(NULL) {
  _updateTime();
}

EventLoop::~EventLoop() {
  delete _waitWatcher;
  delete _signalpipeWatcher;
  _cleanup();
  for (Reapers::iterator it; (it = _reapers.begin()) != _reapers.end();)
    (*it)->onRelease();
  if (_signalpipe[0] != -1)
    close(_signalpipe[0]);
  if (_signalpipe[1] != -1)
    close(_signalpipe[1]);
  delete _selector;
}

void EventLoop::_cleanup() {
  while (!_pendingReapers.empty()) {
    _pendingReapers.front()->onRelease();
    _pendingReapers.pop();
  }
}

void EventLoop::_updateTime() {
  timespec ts;
  if (unlikely(clock_gettime(CLOCK_MONOTONIC, &ts) == -1))
    throw ftpp::OSError(errno, "clock_gettime");
  _time = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

int EventLoop::_backendTimeout() const {
  if (!_deferWatchers.empty())
    return 0;
  if (_timerWatchers.empty())
    return -1;
  time_t timeout = _timerWatchers.begin()->first - _time;
  if (timeout < 0)
    return 0;
  else if (unlikely(std::numeric_limits<int>::max() < timeout))
    return std::numeric_limits<int>::max();
  return timeout;
}

void EventLoop::_runTimer() {
  while (!_timerWatchers.empty()) {
    TimerWatchers::iterator it = _timerWatchers.begin();
    if (_time < it->first)
      break;
    TimerWatcher *watcher = it->second;
    it->second->_isActive = false;
    _timerWatchers.erase(it);
    watcher->onTimeout();
  }
}

void EventLoop::_runDefer() {
  for (size_t i = _deferWatchers.size(); 0 < i; --i) {
    DeferWatcher *watcher = _deferWatchers.front();
    _deferWatchers.pop_front();
    watcher->_isActive = false;
    watcher->onEvent();
  }
}

void EventLoop::_runIOPoll(int timeout) {
  typedef ftpp::Selector::Events Events;
  typedef ftpp::Selector::event_details event_details;
  Events events;
  for (;;) {
    try {
      _selector->select(events, timeout);
      break;
    } catch (ftpp::OSError const &e) {
      switch (e.get_errno()) {
      case EINTR:
        break;
      default:
        throw;
      }
    }
  }
  _updateTime();
  while (!events.empty()) {
    event_details const &details = events.front();
    IOWatchers::iterator it = _ioWatchers.find(details.fd);
    if (it != _ioWatchers.end()) {
      IOWatcher *watcher = it->second;
      if (watcher->_isActive && details.events & ftpp::Selector::EXCEPT)
        watcher->onExcept();
      else {
        if (watcher->_isActive && details.events & ftpp::Selector::READ)
          watcher->onRead();
        if (watcher->_isActive && details.events & ftpp::Selector::WRITE)
          watcher->onWrite();
      }
    }
    events.pop();
  }
}

void EventLoop::operator++() {
  _running = true;
  try {
    ftpp::logger(ftpp::Logger::DEBUG, "EventLoop once");
    _runIOPoll(_backendTimeout());
    _runTimer();
    _runDefer();
    _cleanup();
  } catch (...) {
    _running = false;
    throw;
  }
  _running = false;
}

void EventLoop::run() {
  assert(!_running);
  _stopFlag = false;
  try {
    ftpp::logger(ftpp::Logger::DEBUG, "EventLoop start");
    _updateTime();
    _runTimer();
    for (; likely(!(_stopFlag ||
                    (_timerWatchers.empty() && _ioWatchers.empty() &&
                     _signalWatchers.empty() && _processWatchers.empty())));
         operator++())
      ;
  } catch (...) {
    ftpp::logger(ftpp::Logger::DEBUG, "EventLoop stop");
    throw;
  }
  ftpp::logger(ftpp::Logger::DEBUG, "EventLoop stop");
}

void EventLoop::stop() {
  _stopFlag = true;
}

void EventLoop::_maybeInitSignalpipe() {
  int &pipe_in = _signalpipe[0];
  int &pipe_out = _signalpipe[1];
  if (unlikely(pipe_in == -1 || pipe_out == -1)) {
    if (unlikely(pipe(_signalpipe) == -1))
      throw ftpp::OSError(errno, "pipe");
    try {
      int flags;
      flags = fcntl(pipe_in, F_GETFD);
      if (unlikely(flags == -1))
        throw ftpp::OSError(errno, "fcntl");
      if (unlikely(fcntl(pipe_in, F_SETFD, flags | FD_CLOEXEC) == -1))
        throw ftpp::OSError(errno, "fcntl");
      flags = fcntl(pipe_out, F_GETFD);
      if (unlikely(flags == -1))
        throw ftpp::OSError(errno, "fcntl");
      if (unlikely(fcntl(pipe_out, F_SETFD, flags | FD_CLOEXEC) == -1))
        throw ftpp::OSError(errno, "fcntl");
    } catch (...) {
      close(pipe_in);
      close(pipe_out);
      pipe_in = pipe_out = -1;
      throw;
    }
  }
}

} // namespace ftev
