/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseProcessWatcher.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/30 02:59:32 by hshimizu         ###   ########.fr       */
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

  class WaitWatcher : public BaseSignalWatcher {
  public:
    WaitWatcher(EventLoop &loop);
    ~WaitWatcher();

    void on_signal();
  };

  void _activate();

protected:
  BaseProcessWatcher(EventLoop &loop);

public:
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
