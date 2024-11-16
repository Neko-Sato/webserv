/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSignalWatcher.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 00:22:29 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/16 17:06:48 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop.hpp>
#include <EventLoop/BaseIOWatcher.hpp>
#include <csignal>

namespace ftev {

class EventLoop::BaseSignalWatcher {
protected:
  EventLoop &_loop;

private:
  int _signum;
  sighandler_t _old_handler;
  BaseSignalWatcher *_old_watcher;

  static void _signal_handler(int signum);

  BaseSignalWatcher(BaseSignalWatcher const &rhs);
  BaseSignalWatcher &operator=(BaseSignalWatcher const &rhs);

public:
  BaseSignalWatcher(EventLoop &loop);
  virtual ~BaseSignalWatcher();

  void start(int signum);
  void stop();
  virtual void on_signal() = 0;
};
} // namespace ftev
