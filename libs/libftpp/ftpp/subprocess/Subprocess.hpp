/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Subprocess.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 12:13:15 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 13:32:08 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftpp/noncopyable/NonCopyable.hpp>

#include <csignal>
#include <exception>
#include <sys/types.h>

namespace ftpp {

class Subprocess : NonCopyable {
public:
  class Signaled : public std::exception {
  private:
    int _signum;

  public:
    Signaled(int signum = 0);
    ~Signaled() throw();
    char const *what() const throw();
  };

  struct options {
    char const *path;
    char const *const *argv;
    char const *const *envp;
    char const *cwd;
    struct pipe_pair {
      int dst;
      int src;
    };
    pipe_pair *pipes;
    std::size_t npipes;
  };

  static pid_t spawn(options const &opts);

private:
  pid_t _pid;
  bool _detached;

public:
  Subprocess();
  Subprocess(pid_t pid);
  Subprocess(options const &opts);
  ~Subprocess();

  void swap(Subprocess &rhs) throw();

  pid_t getPid() const;
  void kill(int signum = SIGKILL);
  void detach();
  int wait();
};

} // namespace ftpp
