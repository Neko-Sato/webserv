/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalWatcher.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 00:22:29 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/28 22:15:01 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/IOWatcher.hpp>
#include <ftev/EventLoop/Watcher.hpp>

#include <csignal>

namespace ftev {

class EventLoop::SignalWatcher : public EventLoop::Watcher {
private:
  bool _is_active;
  SignalWatchers::iterator _it;
  sighandler_t _old_handler;

  class SignalpipeWatcher : private IOWatcher {
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
  SignalWatcher(EventLoop &loop);

public:
  virtual ~SignalWatcher();
  void operator()();

  void start(int signum);
  void stop();
  virtual void on_signal() = 0;

  bool is_active() const;
};

} // namespace ftev
