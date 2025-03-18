/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseProcessWatcher.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:23:58 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 19:04:27 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/BaseProcessWatcher.hpp>

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/macros.hpp>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

namespace ftev {

EventLoop::BaseProcessWatcher::BaseProcessWatcher(EventLoop &loop)
    : BaseWatcher(loop) {
}

EventLoop::BaseProcessWatcher::~BaseProcessWatcher() {
}

void EventLoop::BaseProcessWatcher::operator()(int status) {
  if (!_is_active)
    return;
  loop._process_watchers.erase(_it);
  _is_active = false;
  if (WIFEXITED(status))
    on_exited(WEXITSTATUS(status));
  else if (WIFSIGNALED(status))
    on_signaled(WTERMSIG(status));
}

void EventLoop::BaseProcessWatcher::start(options const &opts) {
  assert(!_is_active);
  WaitWatcher::activate(loop);
  pid_t pid = _spawn(opts);
  std::pair<ProcessWatchers::iterator, bool> result =
      loop._process_watchers.insert(std::make_pair(pid, this));
  assert(result.second); /* It shouldn't already be there. */
  _it = result.first;
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

EventLoop::BaseProcessWatcher::WaitWatcher::WaitWatcher(EventLoop &loop)
    : BaseSignalWatcher(loop) {
}

EventLoop::BaseProcessWatcher::WaitWatcher::~WaitWatcher() {
  if (is_active())
    stop();
}

void EventLoop::BaseProcessWatcher::WaitWatcher::activate(EventLoop &loop) {
  if (unlikely(!loop._wait_watcher))
    loop._wait_watcher = new WaitWatcher(loop);
  if (unlikely(!loop._wait_watcher->is_active()))
    loop._wait_watcher->start(SIGCHLD);
}

void EventLoop::BaseProcessWatcher::WaitWatcher::on_signal() {
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

pid_t EventLoop::BaseProcessWatcher::_spawn(options const &opts) {
  pid_t pid = fork();
  if (unlikely(pid == -1))
    throw ftpp::OSError(errno, "fork");
  if (pid)
    return pid;
  try {
    if (opts.cwd && unlikely(chdir(opts.cwd) == -1))
      throw ftpp::OSError(errno, "chdir");
    if (opts.stdin != -1) {
      if (unlikely(dup2(opts.stdin, STDIN_FILENO) == -1))
        throw ftpp::OSError(errno, "dup2");
      if (opts.stdin != 0 && opts.stdin != 1 && opts.stdin != 2)
        close(opts.stdin);
    }
    if (opts.stdout != -1) {
      if (unlikely(dup2(opts.stdout, STDOUT_FILENO) == -1))
        throw ftpp::OSError(errno, "dup2");
      if (opts.stdout != 0 && opts.stdout != 1 && opts.stdout != 2)
        close(opts.stdout);
    }
    if (opts.stderr != -1) {
      if (unlikely(dup2(opts.stderr, STDERR_FILENO) == -1))
        throw ftpp::OSError(errno, "dup2");
      if (opts.stderr != 0 && opts.stderr != 1 && opts.stderr != 2)
        close(opts.stderr);
    }
    execve(opts.file, const_cast<char *const *>(opts.args),
           const_cast<char *const *>(opts.envp ? opts.envp : environ));
    throw ftpp::OSError(errno, "execve");
  } catch (ftpp::OSError const &e) {
    std::cerr << e.what() << std::endl;
  }
  exit(127);
}

} // namespace ftev
