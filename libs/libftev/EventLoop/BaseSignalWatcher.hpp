/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSignalWatcher.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 00:22:29 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/17 23:44:49 by hshimizu         ###   ########.fr       */
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
  sighandler_t _old_handler;

  class SignalpipeWatcher : private BaseIOWatcher {
  private:
    SignalpipeWatcher(EventLoop &loop);

  public:
    static void activate(EventLoop &loop);

    ~SignalpipeWatcher();

    void on_read();
    void on_write();
    void on_except();
  };

  static void _signal_handler(int signum);

protected:
  BaseSignalWatcher(EventLoop &loop);

public:
  virtual ~BaseSignalWatcher();
  void operator()();

  void start(int signum);
  void stop();
  virtual void on_signal() = 0;
};

} // namespace ftev
