/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseProcessWatcher.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/20 05:08:12 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop.hpp>
#include <EventLoop/BaseSignalWatcher.hpp>
#include <EventLoop/BaseWatcher.hpp>

#include <csignal>

namespace ftev {

class EventLoop::BaseProcessWatcher : public EventLoop::BaseWatcher {
private:
  using BaseWatcher::_is_active;
  ProcessWatchers::iterator _it;

  void _activate();
  static void _on_sigchld(BaseSignalWatcher &watcher, int _);

public:
  BaseProcessWatcher(EventLoop &loop);
  virtual ~BaseProcessWatcher();
  void operator()(int status);

  struct options {
    char const *file;
    char const *const *args;
    char const *const *envp;
    char const *cwd;
    int pipe[2];
  };

  void start(options const &opts);
  void kill(int signum = SIGKILL);
  void detach();
  virtual void on_exited(int status) = 0;
  virtual void on_signaled(int signum) = 0;

private:
  static pid_t _spawn(options const &opts);
};

} // namespace ftev
