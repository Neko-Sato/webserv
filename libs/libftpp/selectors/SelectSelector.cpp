/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SelectSelector.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 23:35:50 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/03 21:32:46 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exceptions/OSError.hpp>
#include <macros.hpp>
#include <selectors/SelectSelector.hpp>

#include <sys/select.h>

namespace ftpp {

SelectSelector::SelectSelector() {
}

SelectSelector::~SelectSelector() {
}

void SelectSelector::select(Events &events, int timeout) const {
  fd_set readfds, writefds, exceptfds;
  int maxfd = 0;
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);
  FD_ZERO(&exceptfds);
  for (Mapping::const_iterator it = _fds.begin(); it != _fds.end(); it++) {
    if (maxfd < it->first)
      maxfd = it->first;
    if (it->second & READ)
      FD_SET(it->first, &readfds);
    if (it->second & WRITE)
      FD_SET(it->first, &writefds);
    FD_SET(it->first, &exceptfds);
  }
  int nfds;
  if (timeout < 0)
    nfds = ::select(maxfd + 1, &readfds, &writefds, &exceptfds, NULL);
  else {
    timeval _timeout;
    _timeout.tv_sec = timeout / 1000;
    _timeout.tv_usec = (timeout % 1000) * 1000;
    nfds = ::select(maxfd + 1, &readfds, &writefds, &exceptfds, &_timeout);
  }
  if (unlikely(nfds == -1))
    throw OSError(errno, "select");
  for (Mapping::const_iterator it = _fds.begin(); nfds && it != _fds.end();
       it++) {
    event_details tmp;
    tmp.fd = it->first;
    tmp.events = 0;
    if (FD_ISSET(it->first, &readfds))
      tmp.events |= READ;
    if (FD_ISSET(it->first, &writefds))
      tmp.events |= WRITE;
    if (FD_ISSET(it->first, &exceptfds))
      tmp.events |= EXCEPT;
    if (tmp.events)
      events.push(tmp), --nfds;
  }
}

} // namespace ftpp
