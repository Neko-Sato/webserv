/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseProcessWatcher.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 18:44:52 by hshimizu         ###   ########.fr       */
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
  ProcessWatchers::iterator _it;

  static void _on_sigchld(SignalWatcher<int> &watcher, int _);

public:
  BaseProcessWatcher(EventLoop &loop);
  virtual ~BaseProcessWatcher();
  void operator()(int status);

  void start(pid_t pid);
  void kill(int signum = SIGKILL);
  void detach();
  virtual void on_exit(int status) = 0;
};

} // namespace ftev
