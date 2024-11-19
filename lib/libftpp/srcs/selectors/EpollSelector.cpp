/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollSelector.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:35:30 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/20 04:24:29 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exceptions/OSError.hpp>
#include <selectors/EpollSelector.hpp>

#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

namespace ftpp {

EpollSelector::EpollSelector() {
  _epfd = epoll_create(1);
  if (__glibc_unlikely(_epfd == -1))
    throw OSError(errno, "epoll_create");
  try {
    int flags = fcntl(_epfd, F_GETFD);
    if (__glibc_unlikely(flags == -1))
      throw ftpp::OSError(errno, "fcntl");
    if (__glibc_unlikely(fcntl(_epfd, F_SETFD, flags | FD_CLOEXEC) == -1))
      throw ftpp::OSError(errno, "fcntl");
  } catch (OSError const &e) {
    close(_epfd);
    throw;
  }
}

EpollSelector::~EpollSelector() {
  close(_epfd);
}

void EpollSelector::add(int fd, int events) {
  try {
    struct epoll_event ev;
    ev.events = 0;
    if (events & READ)
      ev.events |= EPOLLIN | EPOLLHUP;
    if (events & WRITE)
      ev.events |= EPOLLOUT;
    if (events & ERROR)
      ev.events |= EPOLLERR;
    ev.data.fd = fd;
    if (__glibc_unlikely(epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev) == -1))
      throw OSError(errno, "epoll_ctl");
  } catch (OSError const &e) {
    switch (e.get_errno()) {
    case EEXIST:
      throw RegisteredError();
    default:
      throw;
    }
  }
}

void EpollSelector::remove(int fd) {
  try {
    if (__glibc_unlikely(epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL) == -1))
      throw OSError(errno, "epoll_ctl");
  } catch (OSError const &e) {
    switch (e.get_errno()) {
    case ENOENT:
      throw NotRegisteredError();
    default:
      throw;
    }
  }
}

void EpollSelector::modify(int fd, int events) {
  try {
    struct epoll_event ev;
    ev.events = 0;
    if (events & READ)
      ev.events |= EPOLLIN | EPOLLHUP;
    if (events & WRITE)
      ev.events |= EPOLLOUT;
    if (events & ERROR)
      ev.events |= EPOLLERR;
    ev.data.fd = fd;
    if (__glibc_unlikely(epoll_ctl(_epfd, EPOLL_CTL_MOD, fd, &ev) == -1))
      throw OSError(errno, "epoll_ctl");
  } catch (OSError const &e) {
    switch (e.get_errno()) {
    case ENOENT:
      throw NotRegisteredError();
    default:
      throw;
    }
  }
}

void EpollSelector::select(Events &events, int timeout) const {
  epoll_event ev[max_events];
  int nfds = epoll_wait(_epfd, ev, max_events, timeout);
  if (__glibc_unlikely(nfds == -1))
    throw OSError(errno, "epoll_wait");
  events.clear();
  for (int i = 0; i < nfds; i++) {
    event_details tmp;
    tmp.fd = ev[i].data.fd;
    tmp.events = 0;
    if (ev[i].events & (EPOLLIN | EPOLLHUP))
      tmp.events |= READ;
    if (ev[i].events & EPOLLOUT)
      tmp.events |= WRITE;
    if (ev[i].events & EPOLLERR)
      tmp.events |= ERROR;
    events.push_back(tmp);
  }
}

} // namespace ftpp
