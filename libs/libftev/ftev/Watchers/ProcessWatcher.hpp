/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProcessWatcher.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/23 00:16:29 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/Watchers/SignalWatcher.hpp>
#include <ftev/Watchers/Watcher.hpp>

#include <csignal>

namespace ftev {

class EventLoop::ProcessWatcher : public EventLoop::Watcher {
private:
  bool _is_active;
  ProcessWatchers::iterator _it;

  class WaitWatcher : private SignalWatcher {
  private:
    WaitWatcher(EventLoop &loop);

  public:
    static void activate(EventLoop &loop);

    ~WaitWatcher();

    void on_signal();
  };

protected:
  ProcessWatcher(EventLoop &loop);

public:
  virtual ~ProcessWatcher();
  void operator()(int status);

  void start(pid_t pid);
  void kill(int signum = SIGKILL);
  void detach();
  virtual void on_exited(int status) = 0;
  virtual void on_signaled(int signum) = 0;

  bool is_active() const;
};

} // namespace ftev
