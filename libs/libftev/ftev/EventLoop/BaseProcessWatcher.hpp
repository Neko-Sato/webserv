/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseProcessWatcher.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/21 03:17:54 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/BaseSignalWatcher.hpp>
#include <ftev/EventLoop/BaseWatcher.hpp>

#include <csignal>

namespace ftev {

class EventLoop::BaseProcessWatcher : public EventLoop::BaseWatcher {
private:
  bool _is_active;
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

  void start(pid_t pid);
  void kill(int signum = SIGKILL);
  void detach();
  virtual void on_exited(int status) = 0;
  virtual void on_signaled(int signum) = 0;

  bool is_active() const;
};

} // namespace ftev
