/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseProcessWatcher.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 02:53:40 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop.hpp>
#include <EventLoop/BaseWatcher.hpp>
#include <Watchers/SignalWatcher.hpp>

#include <csignal>

namespace ftev {

class EventLoop::BaseProcessWatcher : public EventLoop::BaseWatcher {
private:
  bool _is_active;
  pid_t _pid;

  static void _on_sigchld(SignalWatcher<int> &watcher, int _);

public:
  BaseProcessWatcher(EventLoop &loop);
  virtual ~BaseProcessWatcher();
  void operator()(int status);

  void start(pid_t pid);
  void kill(int signum = SIGKILL);
  virtual void on_exit(int status) = 0;
};

} // namespace ftev
