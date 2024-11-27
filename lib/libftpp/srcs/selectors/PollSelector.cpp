/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollSelector.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:35:30 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/28 01:58:40 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exceptions/OSError.hpp>
#include <selectors/PollSelector.hpp>

#include <poll.h>

namespace ftpp {

PollSelector::PollSelector() {
}

PollSelector::~PollSelector() {
}

void PollSelector::select(Events &events, int timeout) const {
  std::size_t size = _fds.size();
  pollfd fds[size];
  {
    pollfd *fd = fds;
    for (std::map<int, event_t>::const_iterator it = _fds.begin();
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
  for (std::size_t i = 0; nfds && i < size; i++) {
    event_details tmp;
    tmp.fd = fds[i].fd;
    tmp.events = 0;
    if (fds[i].revents & POLLIN)
      tmp.events |= READ;
    if (fds[i].revents & POLLOUT)
      tmp.events |= WRITE;
    if (fds[i].revents & (POLLERR | POLLHUP))
      tmp.events |= EXCEPT;
    if (tmp.events)
      events.push(tmp), --nfds;
  }
}

} // namespace ftpp
