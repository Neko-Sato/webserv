/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/15 22:08:51 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <selectors/Selector.hpp>
#include <ctime>
#include <memory>
#include <set>

namespace ftev {

class EventLoop {
public:
  class BaseTimerWatcher;
  class BaseIOWatcher;
//   class BaseSignalWatcher;
//   class BaseProcessWatcher;

private:
  std::auto_ptr<ftpp::BaseSelector> _selector;
  time_t _time;
  bool _running;
  bool _stop_flag;

  typedef std::map<time_t, BaseTimerWatcher *> TimerWatchers;
  TimerWatchers _timer_watchers;
  typedef std::map<int, BaseIOWatcher *> IOWatchers;
  IOWatchers _io_watchers;
  
  void _update_time();
  int _backend_timeout();
  void _run_timer();

  EventLoop(EventLoop const &rhs);
  EventLoop &operator=(EventLoop const &rhs);

  void operator++();

public:
  static EventLoop default_loop;

  EventLoop();
  ~EventLoop();

  void run();
  void stop();
};

} // namespace ftev
