/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:57:51 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 04:13:54 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/EventLoop.hpp>
#include <ftev/Watchers/IOWatcher.hpp>
#include <ftev/Watchers/ProcessWatcher.hpp>
#include <ftev/Watchers/SignalWatcher.hpp>
#include <ftev/Watchers/TimerWatcher.hpp>

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

EventLoop EventLoop::default_loop;

int EventLoop::_signalpipe[2] = {-1, -1};

ftpp::Selector *EventLoop::default_selector_factory() {
  return new ftpp::DefaultSelector;
}

EventLoop::EventLoop(selector_factory_t factory)
    : _selector(factory()), _time(0), _running(false), _stop_flag(false),
      _signalpipe_watcher(NULL), _wait_watcher(NULL) {
  _update_time();
}

EventLoop::~EventLoop() {
  delete _wait_watcher;
  delete _signalpipe_watcher;
  std::for_each(_watchers.begin(), _watchers.end(),
                std::mem_fun(&Watcher::release));
  _cleanup();
  if (_signalpipe[0] != -1)
    close(_signalpipe[0]);
  if (_signalpipe[1] != -1)
    close(_signalpipe[1]);
  delete _selector;
}

void EventLoop::_cleanup() {
  while (!_pending_deletion_watchers.empty()) {
    Watcher *watcher = _pending_deletion_watchers.front();
    if (_watchers.find(watcher) != _watchers.end())
      delete watcher;
    _pending_deletion_watchers.pop();
  }
}

void EventLoop::_update_time() {
  timespec ts;
  if (unlikely(clock_gettime(CLOCK_MONOTONIC, &ts) == -1))
    throw ftpp::OSError(errno, "clock_gettime");
  _time = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

int EventLoop::_backend_timeout() const {
  if (_timer_watchers.empty())
    return -1;
  time_t timeout = _timer_watchers.begin()->first - _time;
  if (timeout < 0)
    return 0;
  else if (unlikely(std::numeric_limits<int>::max() < timeout))
    return std::numeric_limits<int>::max();
  return timeout;
}

void EventLoop::_run_timer() {
  _update_time();
  for (;;) {
    TimerWatchers::iterator it = _timer_watchers.begin();
    if (it == _timer_watchers.end() || _time < it->first)
      break;
    (*it->second)();
  }
}

void EventLoop::_run_io_poll(int timeout) {
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
  while (!events.empty()) {
    event_details const &details = events.front();
    IOWatchers::iterator watcher = _io_watchers.find(details.fd);
    if (watcher != _io_watchers.end())
      (*watcher->second)(details.events);
    events.pop();
  }
}

void EventLoop::operator++() {
  _running = true;
  try {
    ftpp::logger(ftpp::Logger::INFO, "EventLoop once");
    _run_io_poll(_backend_timeout());
    _run_timer();
    _cleanup();
  } catch (...) {
    _running = false;
    throw;
  }
  _running = false;
}

void EventLoop::run() {
  assert(!_running);
  _stop_flag = false;
  try {
    ftpp::logger(ftpp::Logger::INFO, "EventLoop start");
    _run_timer();
    for (; likely(!(_stop_flag ||
                    (_timer_watchers.empty() && _io_watchers.empty() &&
                     _signal_watchers.empty() && _process_watchers.empty())));
         operator++())
      ;
  } catch (...) {
    ftpp::logger(ftpp::Logger::INFO, "EventLoop stop");
    throw;
  }
  ftpp::logger(ftpp::Logger::INFO, "EventLoop stop");
}

void EventLoop::stop() {
  _stop_flag = true;
}

void EventLoop::_maybe_init_signalpipe() {
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
