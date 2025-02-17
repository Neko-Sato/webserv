/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseProcessWatcher.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/17 23:51:49 by hshimizu         ###   ########.fr       */
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

  class WaitWatcher : private BaseSignalWatcher {
  private:
    WaitWatcher(EventLoop &loop);

  public:
    static void activate(EventLoop &loop);

    ~WaitWatcher();

    void on_signal();
  };

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
    int stdin;
    int stdout;
    int stderr;
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
