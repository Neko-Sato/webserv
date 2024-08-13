/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollSelector.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:25:48 by hshimizu          #+#    #+#             */
/*   Updated: 2024/08/13 22:30:30 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "selectors/EpollSelector.hpp"

#include "exceptions/OSError.hpp"

EpollSelector::EpollSelector() {
  __fd = epoll_create(1);
  if (__fd < 0)
    throw OSError(errno);
}

EpollSelector::~EpollSelector() {
  close(__fd);
}

void EpollSelector::add(int fd, int events) {
  int ret;
  epoll_event ev;

  ev.events = 0;
  if (events & READ)
    ev.events |= EPOLLIN;
  if (events & WRITE)
    ev.events |= EPOLLOUT;
  ev.data.fd = fd;
  ret = epoll_ctl(__fd, EPOLL_CTL_ADD, fd, &ev);
  if (ret < 0)
    throw OSError(errno);
  __count++;
}

void EpollSelector::remove(int fd) {
  int ret;

  ret = epoll_ctl(__fd, EPOLL_CTL_DEL, fd, NULL);
  if (ret < 0)
    throw OSError(errno);
  __count--;
}

void EpollSelector::modify(int fd, int events) {
  int ret;
  epoll_event ev;

  ev.events = 0;
  if (events & READ)
    ev.events |= EPOLLIN;
  if (events & WRITE)
    ev.events |= EPOLLOUT;
  ev.data.fd = fd;
  ret = epoll_ctl(__fd, EPOLL_CTL_MOD, fd, &ev);
  if (ret < 0)
    throw OSError(errno);
}

void EpollSelector::wait(std::vector<events> &events, int timeout) {
  int maxevents = __count ? __count : 1;
  epoll_event ev[maxevents];

  int size = epoll_wait(__fd, ev, maxevents, timeout);
  if (size < 0)
    throw OSError(errno);
  events.resize(size);
  for (int i = 0; i < size; i++) {
    events[i].fd = ev[i].data.fd;
    events[i].events = 0;
    if (ev[i].events & EPOLLIN)
      events[i].events |= READ;
    if (ev[i].events & EPOLLOUT)
      events[i].events |= WRITE;
  }
}
