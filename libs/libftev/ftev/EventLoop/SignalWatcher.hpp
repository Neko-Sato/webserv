/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalWatcher.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 00:22:29 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/16 05:10:35 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/IOWatcher.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>

#include <csignal>

namespace ftev {

class EventLoop::SignalWatcher : private ftpp::NonCopyable {
public:
  EventLoop &loop;

private:
  bool _isActive;
  SignalWatchers::iterator _it;

  friend class EventLoop;

  class SignalpipeWatcher : private IOWatcher {
  private:
    SignalpipeWatcher(EventLoop &loop);

  public:
    static void activate(EventLoop &loop);

    ~SignalpipeWatcher();

    void onRead();
    void onWrite();
    void onExcept();
  };

  static void _signalHandler(int signum);

  sighandler_t _oldHandler;

protected:
  SignalWatcher(EventLoop &loop);

public:
  virtual ~SignalWatcher();

  void start(int signum);
  void stop();

  virtual void onSignal() = 0;

  bool getIsActive() const;
};

} // namespace ftev
