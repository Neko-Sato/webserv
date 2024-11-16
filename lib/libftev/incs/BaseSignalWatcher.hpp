/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSignalWatcher.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 00:22:29 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/16 14:51:53 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <BaseIOWatcher.hpp>
#include <EventLoop.hpp>
#include <csignal>

namespace ftev {

class EventLoop::BaseSignalWatcher {
protected:
  EventLoop &_loop;

private:
  int _signum;
  sighandler_t _old_handler;

  static void _signal_handler(int signum);

  BaseSignalWatcher(BaseSignalWatcher const &rhs);
  BaseSignalWatcher &operator=(BaseSignalWatcher const &rhs);

public:
  BaseSignalWatcher(EventLoop &loop);
  virtual ~BaseSignalWatcher();
  void operator()();

  void start(int signum);
  void stop();
  virtual void on_signal() = 0;
};
} // namespace ftev
