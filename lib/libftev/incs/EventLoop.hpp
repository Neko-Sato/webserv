/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/20 04:46:10 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <selectors/BaseSelector.hpp>

#include <csignal>
#include <ctime>
#include <list>
#include <map>

namespace ftev {

class EventLoop {
private:
  ftpp::BaseSelector *_selector;
  time_t _time;
  bool _running;
  bool _stop_flag;

  void _update_time();
  int _backend_timeout();
  void _run_timer();
  void operator++();

  EventLoop(EventLoop const &rhs);
  EventLoop &operator=(EventLoop const &rhs);

public:
  static ftpp::BaseSelector *default_selector_factory();

  template <typename SelectorFactory>
  EventLoop(SelectorFactory selector_factory);
  ~EventLoop();

  void run();
  void stop();

public:
  class BaseWatcher;
  class BaseTimerWatcher;
  class BaseIOWatcher;
  class BaseSignalWatcher;
  class BaseProcessWatcher;

private:
  typedef std::multimap<time_t, BaseTimerWatcher *> TimerWatchers;
  TimerWatchers _timer_watchers;

  typedef std::map<int, BaseIOWatcher *> IOWatchers;
  IOWatchers _io_watchers;

  BaseIOWatcher *_signalpipe_watcher;
  std::map<int, sighandler_t> _old_sighandlers;
  typedef std::multimap<int, BaseSignalWatcher *> SignalWatchers;
  SignalWatchers _signal_watchers;

  BaseSignalWatcher *_wait_watcher;
  typedef std::map<pid_t, BaseProcessWatcher *> ProcessWatchers;
  ProcessWatchers _process_watchers;
};

template <typename SelectorFactory>
EventLoop::EventLoop(SelectorFactory selector_factory)
    : _selector(selector_factory()), _time(0), _running(false),
      _stop_flag(false), _signalpipe_watcher(NULL), _wait_watcher(NULL) {
  _update_time();
}

} // namespace ftev
