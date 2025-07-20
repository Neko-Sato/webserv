/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollSelector.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:35:30 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/20 09:07:56 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if defined(__linux__)

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/macros.hpp>
#include <ftpp/selectors/EpollSelector.hpp>

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
  } catch (...) {
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
  Selector::add(fd, events);
  try {
    epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLHUP | EPOLLERR;
    if (events & READ)
      ev.events |= EPOLLIN;
    if (events & WRITE)
      ev.events |= EPOLLOUT;
    if (unlikely(epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev) == -1))
      throw OSError(errno, "epoll_ctl");
  } catch (...) {
    Selector::remove(fd);
    throw;
  }
}

void EpollSelector::remove(int fd) {
  Selector::remove(fd);
  try {
    if (unlikely(epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL) == -1))
      throw OSError(errno, "epoll_ctl");
  } catch (OSError const &e) {
    UNUSED(e);
  }
}

void EpollSelector::modify(int fd, event_t events) {
  Selector::modify(fd, events);
  epoll_event ev;
  ev.data.fd = fd;
  ev.events = EPOLLHUP | EPOLLERR;
  if (events & READ)
    ev.events |= EPOLLIN;
  if (events & WRITE)
    ev.events |= EPOLLOUT;
  try {
    if (unlikely(epoll_ctl(_epfd, EPOLL_CTL_MOD, fd, &ev) == -1))
      throw OSError(errno, "epoll_ctl");
  } catch (...) {
    Selector::remove(fd);
    throw;
  }
}

void EpollSelector::select(Events &events, int timeout) const {
  typedef std::vector<epoll_event> EpollEvents;
  EpollEvents ev(max_events);
  int nfds = epoll_wait(_epfd, ev.data(), ev.size(), timeout);
  if (unlikely(nfds == -1))
    throw OSError(errno, "epoll_wait");
  ev.resize(nfds);
  for (EpollEvents::const_iterator it = ev.begin(); it != ev.end(); ++it) {
    event_details tmp;
    tmp.fd = it->data.fd;
    tmp.events = 0;
    if (it->events & (EPOLLIN | EPOLLHUP))
      tmp.events |= READ;
    if (it->events & EPOLLOUT)
      tmp.events |= WRITE;
    if (it->events & (EPOLLERR | EPOLLHUP))
      tmp.events |= EXCEPT;
    if (tmp.events)
      events.push(tmp);
  }
}

} // namespace ftpp

#endif
