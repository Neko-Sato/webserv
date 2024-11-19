/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSignalWatcher.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 00:22:29 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/19 22:26:35 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop.hpp>
#include <EventLoop/BaseIOWatcher.hpp>
#include <EventLoop/BaseWatcher.hpp>

#include <csignal>

namespace ftev {

class EventLoop::BaseSignalWatcher : public EventLoop::BaseWatcher {
private:
  using BaseWatcher::_is_active;
  SignalWatchers::iterator _it;

  static int _pipe[2];
  static void _pipe_release();

  void _activate();

  static void _signal_handler(int signum);
  static void _signal_pipe_on_read(BaseIOWatcher &watcher, int _);

public:
  BaseSignalWatcher(EventLoop &loop);
  virtual ~BaseSignalWatcher();
  void operator()();

  void start(int signum);
  void stop();
  virtual void on_signal() = 0;
};
} // namespace ftev