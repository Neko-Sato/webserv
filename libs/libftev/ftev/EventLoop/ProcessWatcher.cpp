/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProcessWatcher.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:23:58 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/16 05:10:47 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/ProcessWatcher.hpp>

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/macros.hpp>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

namespace ftev {

EventLoop::ProcessWatcher::ProcessWatcher(EventLoop &loop)
    : loop(loop), _isActive(false) {
}

EventLoop::ProcessWatcher::~ProcessWatcher() {
  assert(!_isActive);
}

void EventLoop::ProcessWatcher::start(pid_t pid) {
  assert(!_isActive);
  WaitWatcher::activate(loop);
  std::pair<ProcessWatchers::iterator, bool> result =
      loop._processWatchers.insert(std::make_pair(pid, this));
  assert(result.second); /* It shouldn't already be there. */
  _it = result.first;
  _isActive = true;
}

void EventLoop::ProcessWatcher::kill(int signum) {
  assert(_isActive);
  ::kill(_it->first, signum);
}

void EventLoop::ProcessWatcher::detach() {
  assert(_isActive);
  loop._processWatchers.erase(_it);
  _isActive = false;
}

EventLoop::ProcessWatcher::WaitWatcher::WaitWatcher(EventLoop &loop)
    : SignalWatcher(loop) {
}

EventLoop::ProcessWatcher::WaitWatcher::~WaitWatcher() {
  if (getIsActive())
    stop();
}

void EventLoop::ProcessWatcher::WaitWatcher::activate(EventLoop &loop) {
  if (unlikely(!loop._waitWatcher))
    loop._waitWatcher = new WaitWatcher(loop);
  if (unlikely(!loop._waitWatcher->getIsActive()))
    loop._waitWatcher->start(SIGCHLD);
}

void EventLoop::ProcessWatcher::WaitWatcher::onSignal() {
  for (;;) {
    int status;
    pid_t pid;
    for (;;) {
      pid = waitpid(-1, &status, WNOHANG);
      if (pid == 0)
        return;
      else if (unlikely(pid == -1)) {
        if (errno == EINTR)
          continue;
        else if (errno == ECHILD)
          return;
        throw ftpp::OSError(errno, "waitpid");
      }
      break;
    };
    ProcessWatchers::iterator it = loop._processWatchers.find(pid);
    if (it == loop._processWatchers.end())
      continue;
    ProcessWatcher *watcher = it->second;
    loop._processWatchers.erase(it);
    watcher->_isActive = false;
    if (WIFEXITED(status))
      watcher->onExited(WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
      watcher->onSignaled(WTERMSIG(status));
  }
}

bool EventLoop::ProcessWatcher::getIsActive() const {
  return _isActive;
}

} // namespace ftev
