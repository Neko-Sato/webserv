/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollSelector.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:25:48 by hshimizu          #+#    #+#             */
/*   Updated: 2024/08/09 20:33:13 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "selectors/EpollSelector.hpp"

#include "exceptions/OSError.hpp"

EpollSelector::EpollSelector() {
  _fd = epoll_create(1);
  if (_fd < 0) throw OSError(errno);
}

EpollSelector::~EpollSelector() {
  close(_fd);
}

void EpollSelector::add(int fd, int events) {
  int ret;
  epoll_event ev;

  ev.events = 0;
  if (events & READ) ev.events |= EPOLLIN;
  if (events & WRITE) ev.events |= EPOLLOUT;
  ev.data.ptr = NULL;
  ret = epoll_ctl(_fd, EPOLL_CTL_ADD, fd, &ev);
  if (ret < 0) throw OSError(errno);
  _count++;
}

void EpollSelector::remove(int fd) {
  int ret;

  ret = epoll_ctl(_fd, EPOLL_CTL_DEL, fd, NULL);
  if (ret < 0) throw OSError(errno);
  _count--;
}

void EpollSelector::modify(int fd, int events) {
  int ret;
  epoll_event ev;

  ev.events = 0;
  if (events & READ) ev.events |= EPOLLIN;
  if (events & WRITE) ev.events |= EPOLLOUT;
  ev.data.ptr = NULL;
  ret = epoll_ctl(_fd, EPOLL_CTL_MOD, fd, &ev);
  if (ret < 0) throw OSError(errno);
}

std::vector<BaseSelector::events> EpollSelector::wait(int timeout) {
  int maxevents = _count ? _count : 1;
  epoll_event ev[maxevents];

  int size = epoll_wait(_fd, ev, maxevents, timeout);
  if (size < 0) throw OSError(errno);
  std::vector<BaseSelector::events> events(size);
  for (int i = 0; i < size; i++) {
    events[i].fd = ev[i].data.fd;
    events[i].events = 0;
    if (ev[i].events & EPOLLIN) events[i].events |= READ;
    if (ev[i].events & EPOLLOUT) events[i].events |= WRITE;
  }
  return events;
}
