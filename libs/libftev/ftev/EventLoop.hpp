/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/25 22:02:18 by hshimizu         ###   ########.fr       */
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
  class Reaper;
  class DeferWatcher;
  class TimerWatcher;
  class IOWatcher;
  class SignalWatcher;
  class ProcessWatcher;

private:
  ftpp::Selector *_selector;
  time_t _time;
  bool _running;
  bool _stopFlag;

  void _cleanup();
  void _updateTime();
  int _backendTimeout() const;
  void _runTimer();
  void _runDefer();
  void _runIOPoll(int timeout);
  void operator++();

public:
  static EventLoop defaultLoop;

  typedef ftpp::Selector *(selector_factory_t)();
  static ftpp::Selector *defaultSelectorFactory();
  EventLoop(selector_factory_t factory = defaultSelectorFactory);
  ~EventLoop();

  void run();
  void stop();

private:
  // Reaper
  typedef std::set<Reaper *> Reapers;
  Reapers _reapers;
  typedef std::queue<Reaper *> PendingReapers;
  PendingReapers _pendingReapers;

  // Defer watcher
  typedef std::list<DeferWatcher *> DeferWatchers;
  DeferWatchers _deferWatchers;

  // Timer watchers
  typedef std::multimap<time_t, TimerWatcher *> TimerWatchers;
  TimerWatchers _timerWatchers;

  // IOWatcher
  typedef std::map<int, IOWatcher *> IOWatchers;
  IOWatchers _ioWatchers;

  // Signal watcher
  static void _maybeInitSignalpipe();
  static int _signalpipe[2];
  IOWatcher *_signalpipeWatcher;
  typedef std::map<int, SignalWatcher *> SignalWatchers;
  SignalWatchers _signalWatchers;

  // Process watcher
  SignalWatcher *_waitWatcher;
  typedef std::map<pid_t, ProcessWatcher *> ProcessWatchers;
  ProcessWatchers _processWatchers;
};

} // namespace ftev
