/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollSelector.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:35:30 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/01 12:31:59 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exceptions/OSError.hpp>
#include <selectors/EpollSelector.hpp>

#if defined(__linux__)

#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

namespace ftpp {

int const EpollSelector::max_events = 1024;

int EpollSelector::_create_epollfd() {
  int epfd = epoll_create(1);
  if (__glibc_unlikely(epfd == -1))
    throw OSError(errno, "epoll_create");
  try {
    int flags = fcntl(epfd, F_GETFD);
    if (__glibc_unlikely(flags == -1))
      throw ftpp::OSError(errno, "fcntl");
    if (__glibc_unlikely(fcntl(epfd, F_SETFD, flags | FD_CLOEXEC) == -1))
      throw ftpp::OSError(errno, "fcntl");
  } catch (OSError const &e) {
    close(epfd);
    throw;
  }
  return epfd;
}

EpollSelector::EpollSelector() : _epfd(_create_epollfd()) {
}

EpollSelector::~EpollSelector() {
  close(_epfd);
}

void EpollSelector::add(int fd, event_t events) {
  try {
    epoll_event ev;
    ev.events = 0;
    if (events & READ)
      ev.events |= EPOLLIN;
    if (events & WRITE)
      ev.events |= EPOLLOUT;
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
  _fds[fd] = events;
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
  _fds.erase(fd);
}

void EpollSelector::modify(int fd, event_t events) {
  Mapping::iterator it = _fds.find(fd);
  if (it == _fds.end())
    throw NotRegisteredError();
  if (it->second == events)
    return;
  try {
    epoll_event ev;
    ev.events = 0;
    if (events & READ)
      ev.events |= EPOLLIN;
    if (events & WRITE)
      ev.events |= EPOLLOUT;
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
  it->second = events;
}

void EpollSelector::select(Events &events, int timeout) const {
  epoll_event ev[max_events];
  int nfds = epoll_wait(_epfd, ev, max_events, timeout);
  if (__glibc_unlikely(nfds == -1))
    throw OSError(errno, "epoll_wait");
  for (int i = 0; i < nfds; i++) {
    event_details tmp;
    tmp.fd = ev[i].data.fd;
    tmp.events = 0;
    if (ev[i].events & EPOLLIN)
      tmp.events |= READ;
    if (ev[i].events & EPOLLOUT)
      tmp.events |= WRITE;
    if (ev[i].events & (EPOLLERR | EPOLLHUP))
      tmp.events |= EXCEPT;
    events.push(tmp);
  }
}

} // namespace ftpp

#endif
