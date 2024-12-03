/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/04 05:35:03 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <selectors/BaseSelector.hpp>

#include <csignal>
#include <ctime>
#include <list>
#include <map>
#include <queue>

namespace ftev {

/*
    [ unithread event loop ]

    I am a perfectionist.
    But this assignment hates multi-threading.
    So I can't use functions to combat that.
    Damn.
    Please don't use multi-threading,
    although it won't happen...
    When there are multiple event loops,
    the signal watcher cannot be created properly with just the signal
   function. Impossible! That's why the eventloop has to be a singleton. I'll
   keep the constructor secret, so use the default loop.
*/

class EventLoop {
public:
  class BaseWatcher;
  class BaseTimerWatcher;
  class BaseIOWatcher;
  class BaseSignalWatcher;
  class BaseProcessWatcher;

private:
  ftpp::BaseSelector *_selector;
  time_t _time;
  bool _running;
  bool _stop_flag;

  void _update_time();
  int _backend_timeout() const;
  void _run_timer();
  void _run_io_poll(int timeout);
  void _delete_watchers();
  void operator++();

  ~EventLoop();
  EventLoop();

  EventLoop(EventLoop const &rhs);
  EventLoop &operator=(EventLoop const &rhs);

public:
  static EventLoop default_loop;

  void run();
  void stop();

private:
  std::queue<BaseWatcher *> _pending_deletion_watchers;

  typedef std::multimap<time_t, BaseTimerWatcher *> TimerWatchers;
  TimerWatchers _timer_watchers;

  typedef std::map<int, BaseIOWatcher *> IOWatchers;
  IOWatchers _io_watchers;

  static int _signalpipe[2];
  BaseIOWatcher *_signalpipe_watcher;
  typedef std::map<int, BaseSignalWatcher *> SignalWatchers;
  SignalWatchers _signal_watchers;

  BaseSignalWatcher *_wait_watcher;
  typedef std::map<pid_t, BaseProcessWatcher *> ProcessWatchers;
  ProcessWatchers _process_watchers;
};

} // namespace ftev
