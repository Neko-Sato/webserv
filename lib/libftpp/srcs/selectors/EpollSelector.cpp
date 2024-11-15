/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollSelector.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:35:30 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/15 23:57:05 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exceptions/OSError.hpp>
#include <selectors/EpollSelector.hpp>

#include <sys/epoll.h>
#include <unistd.h>

namespace ftpp {

EpollSelector::EpollSelector() {
  _epfd = epoll_create(1);
  if (__glibc_unlikely(_epfd == -1))
    throw OSError(errno, "epoll_create");
}

EpollSelector::~EpollSelector() {
  close(_epfd);
}

void EpollSelector::add(int fd, int events) {
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
}

void EpollSelector::remove(int fd) {
  if (__glibc_unlikely(epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL) == -1))
    throw OSError(errno, "epoll_ctl");
}

void EpollSelector::modify(int fd, int events) {
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
}

void EpollSelector::select(Events &events, int timeout) const {
  epoll_event ev[max_events];
  int nfds = epoll_wait(_epfd, ev, max_events, timeout);
  if (__glibc_unlikely(nfds == -1))
    throw OSError(errno, "epoll_wait");
  events.clear();
  for (int i = 0; i < nfds; i++) {
    event_detals tmp;
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
