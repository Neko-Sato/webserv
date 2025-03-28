/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ProcessWatcher.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:23:58 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/28 22:15:01 by hshimizu         ###   ########.fr       */
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
    : Watcher(loop), _is_active(false) {
}

EventLoop::ProcessWatcher::~ProcessWatcher() {
  assert(!_is_active);
}

void EventLoop::ProcessWatcher::operator()(int status) {
  if (!_is_active)
    return;
  loop._process_watchers.erase(_it);
  _is_active = false;
  if (WIFEXITED(status))
    on_exited(WEXITSTATUS(status));
  else if (WIFSIGNALED(status))
    on_signaled(WTERMSIG(status));
}

void EventLoop::ProcessWatcher::start(pid_t pid) {
  assert(!_is_active);
  WaitWatcher::activate(loop);
  std::pair<ProcessWatchers::iterator, bool> result =
      loop._process_watchers.insert(std::make_pair(pid, this));
  assert(result.second); /* It shouldn't already be there. */
  _it = result.first;
  _is_active = true;
}

void EventLoop::ProcessWatcher::kill(int signum) {
  assert(_is_active);
  ::kill(_it->first, signum);
}

void EventLoop::ProcessWatcher::detach() {
  assert(_is_active);
  loop._process_watchers.erase(_it);
  _is_active = false;
}

EventLoop::ProcessWatcher::WaitWatcher::WaitWatcher(EventLoop &loop)
    : SignalWatcher(loop) {
}

EventLoop::ProcessWatcher::WaitWatcher::~WaitWatcher() {
  if (is_active())
    stop();
}

void EventLoop::ProcessWatcher::WaitWatcher::activate(EventLoop &loop) {
  if (unlikely(!loop._wait_watcher))
    loop._wait_watcher = new WaitWatcher(loop);
  if (unlikely(!loop._wait_watcher->is_active()))
    loop._wait_watcher->start(SIGCHLD);
}

void EventLoop::ProcessWatcher::WaitWatcher::on_signal() {
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
    ProcessWatchers::iterator it = loop._process_watchers.find(pid);
    if (it == loop._process_watchers.end())
      continue;
    (*it->second)(status);
  }
}

bool EventLoop::ProcessWatcher::is_active() const {
  return _is_active;
}

} // namespace ftev
