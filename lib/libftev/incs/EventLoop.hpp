/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 20:47:16 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <selectors/Selector.hpp>

#include <csignal>
#include <ctime>
#include <list>
#include <map>
#include <memory>
#include <set>

namespace ftev {

class EventLoop {
private:
  std::auto_ptr<ftpp::BaseSelector> _selector;
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
  static EventLoop default_loop;

  typedef ftpp::BaseSelector *(*selector_factory_t)();
  static ftpp::BaseSelector *default_selector_factory();

  EventLoop(selector_factory_t selector_factory = default_selector_factory);
  ~EventLoop();

  void run();
  void stop();
  void cleanup();

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

  std::auto_ptr<BaseIOWatcher> _signal_io_watcher;
  std::map<int, sighandler_t> _old_sighandlers;
  typedef std::multimap<int, BaseSignalWatcher *> SignalWatchers;
  SignalWatchers _signal_watchers;

  std::auto_ptr<BaseSignalWatcher> _wait_watcher;
  typedef std::multimap<pid_t, BaseProcessWatcher *> ProcessWatchers;
  ProcessWatchers _process_watchers;
};

} // namespace ftev
