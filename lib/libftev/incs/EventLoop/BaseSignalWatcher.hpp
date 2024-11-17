/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSignalWatcher.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 00:22:29 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 17:53:27 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop.hpp>
#include <EventLoop/BaseWatcher.hpp>
#include <Watchers/IOWatcher.hpp>

#include <csignal>

namespace ftev {

class EventLoop::BaseSignalWatcher : public EventLoop::BaseWatcher {
private:
  bool _is_active;
  SignalWatchers::iterator _it;

  static void _signal_handler(int signum);
  static void _signal_pipe_on_read(IOWatcher<int> &watcher, int _);

public:
  BaseSignalWatcher(EventLoop &loop);
  virtual ~BaseSignalWatcher();
  void operator()();

  void start(int signum);
  void stop();
  virtual void on_signal() = 0;
};
} // namespace ftev
