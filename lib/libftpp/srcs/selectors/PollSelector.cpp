/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollSelector.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:35:30 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/15 03:46:36 by hshimizu         ###   ########.fr       */
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
  _fds.insert(std::make_pair(fd, events));
}

void PollSelector::remove(int fd) {
  std::map<int, unsigned int>::iterator it = _fds.find(fd);
  if (it != _fds.end())
    _fds.erase(it);
}

void PollSelector::modify(int fd, int events) {
  std::map<int, unsigned int>::iterator it = _fds.find(fd);
  if (it != _fds.end())
    it->second = events;
}

void PollSelector::select(std::deque<events> &events, int timeout) const {
  std::size_t size = _fds.size();
  pollfd fds[size];
  for (std::map<int, unsigned int>::const_iterator it = _fds.begin();
       it != _fds.end(); it++) {
    pollfd &fd = fds[it->first];
    fd.fd = it->first;
    fd.events = 0;
    if (it->second & READ)
      fd.events |= (POLLIN | POLLHUP);
    if (it->second & WRITE)
      fd.events |= POLLOUT;
    if (it->second & ERROR)
      fd.events |= POLLERR;
  }
  int nfds = poll(fds, size, timeout);
  if (nfds == -1)
    throw OSError(errno);
  events.clear();
  if (nfds == 0)
    return;
  for (std::size_t i = 0; i < size; i++) {
    struct events tmp;
    tmp.fd = fds[i].fd;
    tmp.events = 0;
    if (fds[i].revents & (POLLIN | POLLHUP))
      tmp.events |= READ;
    if (fds[i].revents & POLLOUT)
      tmp.events |= WRITE;
    if (fds[i].revents & POLLERR)
      tmp.events |= ERROR;
    events.push_back(tmp);
  }
}

} // namespace ftpp
