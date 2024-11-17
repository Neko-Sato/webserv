/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseProcessWatcher.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:23:58 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 20:44:10 by hshimizu         ###   ########.fr       */
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
    : EventLoop::BaseWatcher(loop), _is_active(false) {
}

EventLoop::BaseProcessWatcher::~BaseProcessWatcher() {
  assert(!_is_active);
}

bool EventLoop::BaseProcessWatcher::is_active() const {
  return _is_active;
}

void EventLoop::BaseProcessWatcher::operator()(int status) {
  assert(_is_active);
  loop._process_watchers.erase(_it);
  _is_active = false;
  on_exit(status);
}

void EventLoop::BaseProcessWatcher::start(pid_t pid) {
  assert(!_is_active);
  activate(loop);
  _it = loop._process_watchers.insert(std::make_pair(pid, this));
  _is_active = true;
}

void EventLoop::BaseProcessWatcher::kill(int signum) {
  assert(_is_active);
  ::kill(_it->first, signum);
}

void EventLoop::BaseProcessWatcher::detach() {
  assert(_is_active);
  loop._process_watchers.erase(_it);
  _is_active = false;
}

void EventLoop::BaseProcessWatcher::_on_sigchld(BaseSignalWatcher &watcher,
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
      } catch (ftpp::OSError const &e) {
        switch (e.get_errno()) {
        case EINTR:
          continue;
        case ECHILD:
          return;
        default:
          throw;
        }
      }
    } while (0);
    std::pair<ProcessWatchers::iterator, ProcessWatchers::iterator> range =
        watcher.loop._process_watchers.equal_range(pid);
    ProcessWatchers tmp(range.first, range.second);
    for (ProcessWatchers::iterator it = tmp.begin(); it != tmp.end(); ++it)
      it->second->operator()(status);
  }
}

void EventLoop::BaseProcessWatcher::activate(EventLoop &loop) {
  if (__glibc_unlikely(!loop._wait_watcher.get()))
    loop._wait_watcher.reset(new SignalWatcher<int>(loop, _on_sigchld, 0));
  if (__glibc_unlikely(!loop._wait_watcher->is_active()))
    loop._wait_watcher->start(SIGCHLD);
}

} // namespace ftev
