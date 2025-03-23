/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/23 00:14:06 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftpp/noncopyable/NonCopyable.hpp>
#include <ftpp/selectors/DefaultSelector.hpp>

#include <csignal>
#include <ctime>
#include <list>
#include <map>
#include <queue>
#include <set>

namespace ftev {

// [ unithread event loop ]

// I am a perfectionist.
// But this assignment hates multi-threading.
// So I can't use functions to combat that.
// Damn.
// Please don't use multi-threading,
// although it won't happen...
// When there are multiple event loops,
// the signal watcher cannot be created properly with just the signal
// function. Impossible! That's why the eventloop has to be a singleton. I'll
// keep the constructor secret, so use the default loop.

class EventLoop : private ftpp::NonCopyable {
public:
  class Watcher;
  class TimerWatcher;
  class IOWatcher;
  class SignalWatcher;
  class ProcessWatcher;

private:
  ftpp::Selector *_selector;
  time_t _time;
  bool _running;
  bool _stop_flag;

  void _cleanup();
  void _update_time();
  int _backend_timeout() const;
  void _run_timer();
  void _run_io_poll(int timeout);
  void operator++();

public:
  static EventLoop default_loop;

  typedef ftpp::Selector *(selector_factory_t)();
  static ftpp::Selector *default_selector_factory();
  EventLoop(selector_factory_t factory = default_selector_factory);
  ~EventLoop();

  void run();
  void stop();

private:
  typedef std::queue<Watcher *> PendingDeletionWatchers;
  PendingDeletionWatchers _pending_deletion_watchers;

  typedef std::set<Watcher *> Watchers;
  Watchers _watchers;

  typedef std::multimap<time_t, TimerWatcher *> TimerWatchers;
  TimerWatchers _timer_watchers;

  typedef std::map<int, IOWatcher *> IOWatchers;
  IOWatchers _io_watchers;

  static void _maybe_init_signalpipe();
  static int _signalpipe[2];
  IOWatcher *_signalpipe_watcher;
  typedef std::map<int, SignalWatcher *> SignalWatchers;
  SignalWatchers _signal_watchers;

  SignalWatcher *_wait_watcher;
  typedef std::map<pid_t, ProcessWatcher *> ProcessWatchers;
  ProcessWatchers _process_watchers;
};

} // namespace ftev
