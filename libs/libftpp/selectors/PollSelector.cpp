/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollSelector.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:35:30 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/03 21:58:33 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exceptions/OSError.hpp>
#include <macros.hpp>
#include <selectors/PollSelector.hpp>

#include <poll.h>
#include <vector>

namespace ftpp {

PollSelector::PollSelector() {
}

PollSelector::~PollSelector() {
}

void PollSelector::select(Events &events, int timeout) const {
  typedef std::vector<pollfd> Pollfds;
  Pollfds fds;
  fds.reserve(_fds.size());
  for (Mapping::const_iterator it = _fds.begin(); it != _fds.end(); ++it) {
    pollfd fd;
    fd.fd = it->first;
    fd.events = POLLERR | POLLHUP;
    if (it->second & READ)
      fd.events |= POLLIN;
    if (it->second & WRITE)
      fd.events |= POLLOUT;
    fds.push_back(fd);
  }
  int nfds = poll(fds.data(), fds.size(), timeout);
  if (unlikely(nfds == -1))
    throw OSError(errno, "poll");
  for (Pollfds::iterator it = fds.begin(); nfds && it != fds.end(); ++it) {
    event_details tmp;
    tmp.fd = it->fd;
    tmp.events = 0;
    if (it->revents & POLLIN)
      tmp.events |= READ;
    if (it->revents & POLLOUT)
      tmp.events |= WRITE;
    if (it->revents & (POLLERR | POLLHUP))
      tmp.events |= EXCEPT;
    if (tmp.events)
      events.push(tmp), --nfds;
  }
}

} // namespace ftpp
