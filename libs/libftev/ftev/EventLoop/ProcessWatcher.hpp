/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProcessWatcher.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/16 05:10:50 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/SignalWatcher.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>

#include <csignal>

namespace ftev {

class EventLoop::ProcessWatcher : private ftpp::NonCopyable {
public:
  EventLoop &loop;

private:
  bool _isActive;
  ProcessWatchers::iterator _it;

  friend class EventLoop;

  class WaitWatcher : private SignalWatcher {
  private:
    WaitWatcher(EventLoop &loop);

  public:
    static void activate(EventLoop &loop);

    ~WaitWatcher();

    void onSignal();
  };

protected:
  ProcessWatcher(EventLoop &loop);

public:
  virtual ~ProcessWatcher();

  void start(pid_t pid);
  void kill(int signum = SIGKILL);
  void detach();

  virtual void onExited(int status) = 0;
  virtual void onSignaled(int signum) = 0;

  bool getIsActive() const;
};

} // namespace ftev
