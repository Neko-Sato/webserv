/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/16 14:15:07 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ctime>
#include <memory>
#include <selectors/Selector.hpp>
#include <set>

namespace ftev {

class EventLoop {
public:
  // Base Watchers
  class BaseTimerWatcher;
  class BaseIOWatcher;
  class BaseSignalWatcher;
  //   class BaseProcessWatcher;

private:
  // Private Members
  std::auto_ptr<ftpp::BaseSelector> _selector;
  time_t _time;
  bool _running;
  bool _stop_flag;

  // Members for Watchers
  typedef std::map<time_t, BaseTimerWatcher *> TimerWatchers;
  TimerWatchers _timer_watchers;

  typedef std::map<int, BaseIOWatcher *> IOWatchers;
  IOWatchers _io_watchers;

  class SignalIOWatcher;
  static int _signal_pipe[2];
  static void _acquire_signal_pipe();
  static void _release_signal_pipe();
  std::auto_ptr<SignalIOWatcher> _signal_io_watcher;
  typedef std::map<int, BaseSignalWatcher *> SignalWatchers;
  SignalWatchers _signal_watchers;

  // Private Methods
  void _update_time();
  int _backend_timeout();
  void _run_timer();
  void operator++();

  // Invalid Methods
  EventLoop(EventLoop const &rhs);
  EventLoop &operator=(EventLoop const &rhs);

public:
  static EventLoop default_loop;

  EventLoop();
  ~EventLoop();

  void run();
  void stop();
};

} // namespace ftev
