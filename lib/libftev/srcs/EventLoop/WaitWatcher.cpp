/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WaitWatcher.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:43:50 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/16 17:38:31 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseProcessWatcher.hpp>
#include <EventLoop/WaitWatcher.hpp>
#include <cassert>
#include <exceptions/OSError.hpp>
#include <sys/wait.h>

namespace ftev {

EventLoop::WaitWatcher::WaitWatcher(EventLoop &loop)
    : EventLoop::BaseSignalWatcher(loop) {
}

EventLoop::WaitWatcher::~WaitWatcher() {
}

void EventLoop::WaitWatcher::start() {
  EventLoop::BaseSignalWatcher::start(SIGCHLD);
}

void EventLoop::WaitWatcher::on_signal() {
  while (true) {
    int status;
    pid_t pid;
    do {
      try {
        pid = waitpid(-1, &status, WNOHANG);
        if (__glibc_unlikely(pid == 0))
          return;
        else if (__glibc_unlikely(pid == -1))
          throw ftpp::OSError(errno, "waitpid");
      } catch (const std::exception &e) {
        if (errno == EINTR)
          continue;
        throw;
      }
    } while (0);
    EventLoop::ProcessWatchers::iterator it = _loop._process_watchers.find(pid);
    if (it == _loop._process_watchers.end())
      continue;
    _loop._process_watchers.erase(it);
    it->second->on_exit(status);
  }
}

} // namespace ftev
