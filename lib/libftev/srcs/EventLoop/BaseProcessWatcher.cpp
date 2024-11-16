/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseProcessWatcher.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:23:58 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 07:57:55 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseProcessWatcher.hpp>
#include <Watchers/SignalWatcher.hpp>
#include <exceptions/OSError.hpp>

#include <cassert>
#include <sys/wait.h>

namespace ftev {

EventLoop::BaseProcessWatcher::BaseProcessWatcher(EventLoop &loop)
    : EventLoop::BaseWatcher(loop), _is_active(false), _pid(-1) {
  if (!loop._wait_watcher.get()) {
    loop._wait_watcher.reset(new SignalWatcher<int>(loop, _on_sigchld, 0));
    loop._wait_watcher->start(SIGCHLD);
  }
}

EventLoop::BaseProcessWatcher::~BaseProcessWatcher() {
  assert(!_is_active);
}

void EventLoop::BaseProcessWatcher::operator()(int status) {
  assert(_is_active);
  _is_active = false;
  on_exit(status);
}

void EventLoop::BaseProcessWatcher::start(pid_t pid) {
  assert(!_is_active);
  _pid = pid;
  loop._process_watchers[_pid] = this;
  _is_active = true;
}

void EventLoop::BaseProcessWatcher::kill(int signum) {
  assert(_is_active);
  ::kill(_pid, signum);
}

void EventLoop::BaseProcessWatcher::_on_sigchld(SignalWatcher<int> &watcher,
                                                int _) {
  (void)_;
  while (true) {
    int status;
    pid_t pid;
    do {
      try {
        pid = waitpid(-1, &status, WNOHANG);
        if (pid == 0)
          return;
        else if (__glibc_unlikely(pid == -1))
          throw ftpp::OSError(errno, "waitpid");
      } catch (const std::exception &e) {
        if (errno == EINTR)
          continue;
        else if (errno == ECHILD)
          return;
        throw;
      }
    } while (0);
    EventLoop::BaseProcessWatcher *process_watcher;
    {
      EventLoop::ProcessWatchers::iterator it =
          watcher.loop._process_watchers.find(pid);
      if (it == watcher.loop._process_watchers.end())
        continue;
      process_watcher = it->second;
      watcher.loop._process_watchers.erase(it);
    }
    process_watcher->operator()(status);
  }
}

} // namespace ftev
