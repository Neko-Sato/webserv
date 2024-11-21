/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollSelector.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:35:30 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/21 17:34:09 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exceptions/OSError.hpp>
#include <selectors/PollSelector.hpp>

namespace ftpp {

PollSelector::PollSelector() : _fds() {
}

PollSelector::~PollSelector() {
}

void PollSelector::add(int fd, int events) {
  if (!_fds.insert(std::make_pair(fd, events)).second)
    throw RegisteredError();
}

void PollSelector::remove(int fd) {
  std::map<int, unsigned int>::iterator it = _fds.find(fd);
  if (it == _fds.end())
    throw NotRegisteredError();
  _fds.erase(it);
}

void PollSelector::modify(int fd, int events) {
  std::map<int, unsigned int>::iterator it = _fds.find(fd);
  if (it == _fds.end())
    throw NotRegisteredError();
  it->second = events;
}

void PollSelector::select(Events &events, int timeout) const {
  std::size_t size = _fds.size();
  pollfd fds[size];
  {
    pollfd *fd = fds;
    for (std::map<int, unsigned int>::const_iterator it = _fds.begin();
         it != _fds.end(); it++, fd++) {
      fd->fd = it->first;
      fd->events = POLLERR | POLLHUP;
      if (it->second & READ)
        fd->events |= POLLIN;
      if (it->second & WRITE)
        fd->events |= POLLOUT;
    }
  }
  int nfds = poll(fds, size, timeout);
  if (__glibc_unlikely(nfds == -1))
    throw OSError(errno, "poll");
  events.clear();
  if (nfds == 0)
    return;
  for (std::size_t i = 0; i < size; i++) {
    event_details tmp;
    tmp.fd = fds[i].fd;
    tmp.events = 0;
    if (fds[i].revents & POLLIN)
      tmp.events |= READ;
    if (fds[i].revents & POLLOUT)
      tmp.events |= WRITE;
    if (fds[i].revents & (POLLERR | POLLHUP))
      tmp.events |= EXCEPT;
    events.push_back(tmp);
  }
}

} // namespace ftpp
