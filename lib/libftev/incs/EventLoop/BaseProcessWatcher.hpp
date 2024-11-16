/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseProcessWatcher.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/16 17:06:41 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop.hpp>
#include <csignal>
#include <ctime>

namespace ftev {

class EventLoop::BaseProcessWatcher {
protected:
  EventLoop &_loop;

private:
  pid_t _pid;

  BaseProcessWatcher(BaseProcessWatcher const &rhs);
  BaseProcessWatcher &operator=(BaseProcessWatcher const &rhs);

public:
  BaseProcessWatcher(EventLoop &loop);
  virtual ~BaseProcessWatcher();

  void start(pid_t pid);
  void kill(int signum = SIGKILL);
  virtual void on_exit(int status) = 0;
};

} // namespace ftev
