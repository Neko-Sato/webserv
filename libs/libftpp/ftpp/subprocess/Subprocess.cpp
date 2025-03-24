/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Subprocess.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:13:15 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 13:33:42 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/macros.hpp>
#include <ftpp/subprocess/Subprocess.hpp>

#include <cassert>
#include <cstring>
#include <iostream>
#include <signal.h>
#include <stdexcept>
#include <sys/wait.h>
#include <unistd.h>

namespace ftpp {

Subprocess::Signaled::Signaled(int signum) : _signum(signum) {
}

Subprocess::Signaled::~Signaled() throw() {
}

char const *Subprocess::Signaled::what() const throw() {
  return strsignal(_signum);
}

Subprocess::Subprocess() : _pid(-1), _detached(true) {
}

Subprocess::Subprocess(pid_t pid) : _pid(pid), _detached(false) {
  if (0 < _pid)
    throw std::logic_error("invalid pid");
}

Subprocess::Subprocess(options const &opts)
    : _pid(spawn(opts)), _detached(false) {
}

Subprocess::~Subprocess() {
  if (!_detached) {
    try {
      kill(SIGKILL);
      wait();
    } catch (...) {
    }
  }
}

void Subprocess::swap(Subprocess &rhs) throw() {
  std::swap(_pid, rhs._pid);
  std::swap(_detached, rhs._detached);
}

int Subprocess::getPid() const {
  if (_detached)
    throw std::runtime_error("no process");
  return _pid;
}

void Subprocess::kill(int signum) {
  if (_detached)
    throw std::runtime_error("no process");
  if (unlikely(::kill(_pid, signum) == -1))
    throw OSError(errno, "kill");
}

void Subprocess::detach() {
  if (_detached)
    throw std::runtime_error("no process");
  _detached = true;
}

int Subprocess::wait() {
  if (_detached)
    throw std::runtime_error("no process");
  int status;
  if (unlikely(::waitpid(_pid, &status, 0) == -1))
    throw OSError(errno, "waitpid");
  if (WIFEXITED(status))
    return WEXITSTATUS(status);
  if (WIFSIGNALED(status))
    throw Signaled(WTERMSIG(status));
  assert(false);
}

pid_t Subprocess::spawn(options const &opts) {
  pid_t pid = ::fork();
  if (unlikely(pid == -1))
    throw OSError(errno, "fork");
  if (pid)
    return pid;
  try {
    if (opts.cwd && unlikely(chdir(opts.cwd) == -1))
      throw OSError(errno, "chdir");
    for (std::size_t i = 0; i < opts.npipes; ++i) {
      if (unlikely(dup2(opts.pipes[i].src, opts.pipes[i].dst) == -1))
        throw OSError(errno, "dup2");
      close(opts.pipes[i].src);
    }
    execve(opts.path, const_cast<char *const *>(opts.argv),
           const_cast<char *const *>(opts.envp));
    throw OSError(errno, "execve");
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
  }
  ::_exit(127);
}

} // namespace ftpp
