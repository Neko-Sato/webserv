/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:57:51 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/16 18:07:59 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseIOWatcher.hpp>
#include <EventLoop/BaseTimerWatcher.hpp>
#include <cassert>
#include <cstdlib>
#include <exceptions/OSError.hpp>
#include <limits>
#include <unistd.h>

namespace ftev {

EventLoop EventLoop::default_loop;

ftpp::BaseSelector *EventLoop::default_selector_factory() {
  return new ftpp::Selector();
}

EventLoop::EventLoop(selector_factory_t selector_factory)
    : _selector(selector_factory()), _time(0), _running(false),
      _stop_flag(true), _signal_io_watcher(NULL), _wait_watcher(NULL) {
  _update_time();
}

EventLoop::EventLoop(EventLoop const &rhs) {
  (void)rhs;
  assert(false);
}

EventLoop &EventLoop::operator=(EventLoop const &rhs) {
  (void)rhs;
  assert(false);
  return *this;
}

EventLoop::~EventLoop() {
}

int EventLoop::_signal_pipe[2] = {-1, -1};

void EventLoop::_acquire_signal_pipe() {
  if (_signal_pipe[0] != -1)
    return;
  if (__glibc_unlikely(pipe(_signal_pipe) == -1))
    throw ftpp::OSError(errno, "pipe");
  atexit(_release_signal_pipe);
}

void EventLoop::_release_signal_pipe() {
  if (_signal_pipe[0] != -1) {
    close(_signal_pipe[0]);
    close(_signal_pipe[1]);
    _signal_pipe[0] = -1;
    _signal_pipe[1] = -1;
  }
}

void EventLoop::_update_time() {
  struct timespec ts;
  if (__glibc_unlikely(clock_gettime(CLOCK_MONOTONIC, &ts) == -1))
    throw ftpp::OSError(errno, "clock_gettime");
  _time = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

int EventLoop::_backend_timeout() {
  if (_timer_watchers.empty())
    return -1;
  time_t timeout = _timer_watchers.begin()->first - _time;
  if (timeout < 0)
    return 0;
  else if (__glibc_unlikely(std::numeric_limits<int>::max() < timeout))
    return std::numeric_limits<int>::max();
  return timeout;
}

void EventLoop::_run_timer() {
  TimerWatchers::iterator end = _timer_watchers.lower_bound(_time);
  TimerWatchers tmp(_timer_watchers.begin(), end);
  _timer_watchers.erase(_timer_watchers.begin(), end);
  for (TimerWatchers::iterator it = tmp.begin(); it != tmp.end(); ++it)
    it->second->on_timeout();
}

void EventLoop::operator++() {
  typedef typename ftpp::BaseSelector::Events Events;
  _running = true;
  Events events;
  _update_time();
  do {
    try {
      _selector->select(events, _backend_timeout());
    } catch (ftpp::OSError const &e) {
      if (e.get_errno() != EINTR)
        throw;
      continue;
    }
  } while (0);
  for (Events::iterator it = events.begin(); it != events.end(); ++it)
    _io_watchers[it->fd]->on_event(*it);
  _update_time();
  _run_timer();
  _running = false;
}

void EventLoop::run() {
  assert(!_running);
  _stop_flag = false;
  while (__glibc_likely(!_stop_flag))
    operator++();
}

void EventLoop::stop() {
  _stop_flag = true;
}

} // namespace ftev
