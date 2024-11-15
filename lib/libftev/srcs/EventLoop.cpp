/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:57:51 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/16 00:10:17 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <BaseTimerWatcher.hpp>
#include <BaseIOWatcher.hpp>
#include <EventLoop.hpp>
#include <cassert>
#include <exceptions/OSError.hpp>
#include <limits>

namespace ftev {

EventLoop EventLoop::default_loop;

EventLoop::EventLoop()
    : _selector(new ftpp::Selector), _time(0), _running(false),
      _stop_flag(true) {
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

#include <iostream>
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
    it->second->operator()();
}

void EventLoop::operator++() {
  typedef typename ftpp::BaseSelector::Events Events;
  _running = true;
  Events events;
  _update_time();
  _selector->select(events, _backend_timeout());
  for (Events::iterator it = events.begin(); it != events.end(); ++it)
    _io_watchers[it->fd]->operator()(*it);
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
