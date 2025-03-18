/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 19:04:42 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftpp/noncopyable/NonCopyable.hpp>
#include <ftpp/selectors/BaseSelector.hpp>
#include <ftpp/smart_ptr/ScopedPtr.hpp>

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
  class BaseWatcher;
  class BaseTimerWatcher;
  class BaseIOWatcher;
  class BaseSignalWatcher;
  class BaseProcessWatcher;

private:
  ftpp::ScopedPtr<ftpp::BaseSelector> _selector;
  time_t _time;
  bool _running : 1;
  bool _stop_flag : 1;

  void _update_time();
  int _backend_timeout() const;
  void _run_timer();
  void _run_io_poll(int timeout);
  void operator++();

public:
  static EventLoop default_loop;

  typedef ftpp::BaseSelector *(selector_factory_t)();
  static ftpp::BaseSelector *default_selector_factory();
  EventLoop(selector_factory_t factory = default_selector_factory);
  ~EventLoop();

  void run();
  void stop();

private:
  typedef std::queue<BaseWatcher *> PendingDeletionWatchers;
  PendingDeletionWatchers _pending_deletion_watchers;

  typedef std::set<BaseWatcher *> Watchers;
  Watchers _watchers;

  typedef std::multimap<time_t, BaseTimerWatcher *> TimerWatchers;
  TimerWatchers _timer_watchers;

  typedef std::map<int, BaseIOWatcher *> IOWatchers;
  IOWatchers _io_watchers;

  static void _maybe_init_signalpipe();
  static int _signalpipe[2];
  BaseIOWatcher *_signalpipe_watcher;
  typedef std::map<int, BaseSignalWatcher *> SignalWatchers;
  SignalWatchers _signal_watchers;

  BaseSignalWatcher *_wait_watcher;
  typedef std::map<pid_t, BaseProcessWatcher *> ProcessWatchers;
  ProcessWatchers _process_watchers;
};

} // namespace ftev
