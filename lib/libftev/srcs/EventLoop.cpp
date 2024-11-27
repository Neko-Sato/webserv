/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:57:51 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/28 01:51:31 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseIOWatcher.hpp>
#include <EventLoop/BaseProcessWatcher.hpp>
#include <EventLoop/BaseSignalWatcher.hpp>
#include <EventLoop/BaseTimerWatcher.hpp>
#include <exceptions/OSError.hpp>
#include <selectors/Selector.hpp>

#include <cassert>
#include <cstdlib>
#include <limits>
#include <unistd.h>

namespace ftev {

EventLoop EventLoop::default_loop;

int EventLoop::_signalpipe[2] = {-1, -1};

EventLoop::EventLoop()
    : _selector(new ftpp::Selector), _time(0), _running(false),
      _stop_flag(false), _signalpipe_watcher(NULL), _wait_watcher(NULL) {
  _update_time();
}

EventLoop::~EventLoop() {
  for (TimerWatchers::iterator it = _timer_watchers.begin();
       it != _timer_watchers.end();)
    it++->second->cancel();
  for (IOWatchers::iterator it = _io_watchers.begin();
       it != _io_watchers.end();)
    it++->second->stop();
  for (SignalWatchers::iterator it = _signal_watchers.begin();
       it != _signal_watchers.end();)
    it++->second->stop();
  for (ProcessWatchers::iterator it = _process_watchers.begin();
       it != _process_watchers.end();)
    it++->second->detach();
  delete _signalpipe_watcher;
  delete _wait_watcher;
  delete _selector;
  if (_signalpipe[0] != -1)
    close(_signalpipe[0]);
  if (_signalpipe[1] != -1)
    close(_signalpipe[1]);
}

void EventLoop::_update_time() {
  timespec ts;
  if (__glibc_unlikely(clock_gettime(CLOCK_MONOTONIC, &ts) == -1))
    throw ftpp::OSError(errno, "clock_gettime");
  _time = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

int EventLoop::_backend_timeout() const {
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
  for (;;) {
    TimerWatchers::iterator it = _timer_watchers.begin();
    if (it == _timer_watchers.end() || _time < it->first)
      break;
    it->second->operator()();
  }
}

void EventLoop::operator++() {
  _running = true;
  typedef ftpp::BaseSelector::Events Events;
  typedef ftpp::BaseSelector::event_details event_details;
  Events events;
  for (;;) {
    try {
      _selector->select(events, _backend_timeout());
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
      watcher->second->operator()(details);
    events.pop();
  }
  _run_timer();
  _running = false;
}

void EventLoop::run() {
  assert(!_running);
  _stop_flag = false;
  _run_timer();
  for (;
       __glibc_likely(!_stop_flag ||
                      (_timer_watchers.empty() && _io_watchers.empty() &&
                       _signal_watchers.empty() && _process_watchers.empty()));
       operator++())
    ;
}

void EventLoop::stop() {
  _stop_flag = true;
}

} // namespace ftev
