/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollSelector.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:35:30 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/03 21:55:40 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exceptions/OSError.hpp>
#include <macros.hpp>
#include <selectors/EpollSelector.hpp>

#if defined(__linux__)

#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

namespace ftpp {

int const EpollSelector::max_events = 1024;

int EpollSelector::_create_epollfd() {
  int epfd = epoll_create(1);
  if (unlikely(epfd == -1))
    throw OSError(errno, "epoll_create");
  try {
    int flags = fcntl(epfd, F_GETFD);
    if (unlikely(flags == -1))
      throw ftpp::OSError(errno, "fcntl");
    if (unlikely(fcntl(epfd, F_SETFD, flags | FD_CLOEXEC) == -1))
      throw ftpp::OSError(errno, "fcntl");
  } catch (OSError const &e) {
    close(epfd);
    throw;
  }
  return epfd;
}

EpollSelector::EpollSelector() {
  _epfd = _create_epollfd();
}

EpollSelector::~EpollSelector() {
  close(_epfd);
}

void EpollSelector::add(int fd, event_t events) {
  events &= READ | WRITE;
  try {
    epoll_event ev;
    ev.events = 0;
    if (events & READ)
      ev.events |= EPOLLIN;
    if (events & WRITE)
      ev.events |= EPOLLOUT;
    ev.data.fd = fd;
    if (unlikely(epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev) == -1))
      throw OSError(errno, "epoll_ctl");
  } catch (OSError const &e) {
    switch (e.get_errno()) {
    case EEXIST:
      throw RegisteredError();
    default:
      throw;
    }
  }
  _fds[fd] = events;
}

void EpollSelector::remove(int fd) {
  try {
    if (unlikely(epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL) == -1))
      throw OSError(errno, "epoll_ctl");
  } catch (OSError const &e) {
    switch (e.get_errno()) {
    case ENOENT:
      throw NotRegisteredError();
    default:
      throw;
    }
  }
  _fds.erase(fd);
}

void EpollSelector::modify(int fd, event_t events) {
  events &= READ | WRITE;
  try {
    epoll_event ev;
    ev.events = 0;
    if (events & READ)
      ev.events |= EPOLLIN;
    if (events & WRITE)
      ev.events |= EPOLLOUT;
    ev.data.fd = fd;
    if (unlikely(epoll_ctl(_epfd, EPOLL_CTL_MOD, fd, &ev) == -1))
      throw OSError(errno, "epoll_ctl");
  } catch (OSError const &e) {
    switch (e.get_errno()) {
    case ENOENT:
      throw NotRegisteredError();
    default:
      throw;
    }
  }
  _fds[fd] = events;
}

void EpollSelector::select(Events &events, int timeout) const {
  typedef std::vector<epoll_event> Epoll_events;
  Epoll_events ev(max_events);
  int nfds = epoll_wait(_epfd, ev.data(), ev.size(), timeout);
  if (unlikely(nfds == -1))
    throw OSError(errno, "epoll_wait");
  ev.resize(nfds);
  for (Epoll_events::iterator it = ev.begin(); it != ev.end(); ++it) {
    event_details tmp;
    tmp.fd = it->data.fd;
    tmp.events = 0;
    if (it->events & EPOLLIN)
      tmp.events |= READ;
    if (it->events & EPOLLOUT)
      tmp.events |= WRITE;
    if (it->events & (EPOLLERR | EPOLLHUP))
      tmp.events |= EXCEPT;
    events.push(tmp);
  }
}

} // namespace ftpp

#endif
