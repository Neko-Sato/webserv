/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:57:51 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 06:48:09 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseIOWatcher.hpp>
#include <EventLoop/BaseTimerWatcher.hpp>
#include <exceptions/OSError.hpp>

#include <cassert>
#include <cstdlib>
#include <limits>
#include <unistd.h>

namespace ftev {

EventLoop EventLoop::default_loop;
std::auto_ptr<EventLoop::SignalPipe> EventLoop::_signal_pipe;

ftpp::BaseSelector *EventLoop::default_selector_factory() {
  return new ftpp::Selector();
}

EventLoop::EventLoop(selector_factory_t selector_factory)
    : _selector(selector_factory()), _time(0), _running(false),
      _stop_flag(true) {
  _update_time();
}

EventLoop::~EventLoop() {
  if (_signal_io_watcher.get())
    _signal_io_watcher->stop();
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
  _update_time();
  TimerWatchers::iterator begin = _timer_watchers.begin();
  TimerWatchers::iterator end = _timer_watchers.lower_bound(_time);
  TimerWatchers tmp(begin, end);
  _timer_watchers.erase(begin, end);
  for (TimerWatchers::iterator it = tmp.begin(); it != tmp.end(); ++it)
    it->second->operator()();
}

void EventLoop::operator++() {
  _running = true;
  typedef typename ftpp::BaseSelector::Events Events;
  Events events;
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
    _io_watchers[it->fd]->operator()(*it);
  _run_timer();
  _running = false;
}

void EventLoop::run() {
  assert(!_running);
  _stop_flag = false;
  _run_timer();
  while (__glibc_likely(!_stop_flag))
    operator++();
}

void EventLoop::stop() {
  _stop_flag = true;
}

} // namespace ftev
