/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SelectSelector.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 23:35:50 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/15 19:44:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exceptions/OSError.hpp>
#include <selectors/SelectSelector.hpp>

#include <sys/select.h>

namespace ftpp {

SelectSelector::SelectSelector() : _fds() {
}

SelectSelector::~SelectSelector() {
}

void SelectSelector::add(int fd, int events) {
  _fds.insert(std::make_pair(fd, events));
}

void SelectSelector::remove(int fd) {
  std::map<int, unsigned int>::iterator it = _fds.find(fd);
  if (it != _fds.end())
    _fds.erase(it);
}

void SelectSelector::modify(int fd, int events) {
  std::map<int, unsigned int>::iterator it = _fds.find(fd);
  if (it != _fds.end())
    it->second = events;
}

void SelectSelector::select(Events &events, int timeout) const {
  fd_set readfds, writefds, exceptfds;
  struct timeval _timeout = {};
  int maxfd = 0;
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);
  FD_ZERO(&exceptfds);
  for (std::map<int, unsigned int>::const_iterator it = _fds.begin();
       it != _fds.end(); it++) {
    if (maxfd < it->first)
      maxfd = it->first;
    if (it->second & READ)
      FD_SET(it->first, &readfds);
    if (it->second & WRITE)
      FD_SET(it->first, &writefds);
    if (it->second & ERROR)
      FD_SET(it->first, &exceptfds);
  }
  if (timeout >= 0) {
    _timeout.tv_sec = timeout / 1000;
    _timeout.tv_usec = (timeout % 1000) * 1000;
  }
  int nfds = ::select(maxfd + 1, &readfds, &writefds, &exceptfds, &_timeout);
  if (__glibc_unlikely(nfds == -1))
    throw OSError(errno, "select");
  events.clear();
  if (nfds == 0)
    return;
  for (std::map<int, unsigned int>::const_iterator it = _fds.begin();
       it != _fds.end(); it++) {
    event_detals tmp;
    tmp.fd = it->first;
    tmp.events = 0;
    if (FD_ISSET(it->first, &readfds))
      tmp.events |= READ;
    if (FD_ISSET(it->first, &writefds))
      tmp.events |= WRITE;
    if (FD_ISSET(it->first, &exceptfds))
      tmp.events |= ERROR;
    if (tmp.events)
      events.push_back(tmp);
  }
}

} // namespace ftpp
