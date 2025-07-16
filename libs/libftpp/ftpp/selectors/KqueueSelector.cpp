/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KqueueSelector.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:35:30 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/17 05:25:14 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if defined(__APPLE__) || defined(__FreeBSD__)

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/macros.hpp>
#include <ftpp/selectors/KqueueSelector.hpp>

#include <stdexcept>
#include <sys/event.h>
#include <unistd.h>
#include <vector>

namespace ftpp {

int const KqueueSelector::max_events = 1024;

int KqueueSelector::_create_kqueue() {
  int kq = kqueue();
  if (unlikely(kq == -1))
    throw OSError(errno, "kqueue");
  return kq;
}

KqueueSelector::KqueueSelector() {
  _kq = _create_kqueue();
}

KqueueSelector::~KqueueSelector() {
  close(_kq);
}

void KqueueSelector::add(int fd, event_t events) {
  Selector::add(fd, events);
  try {
    struct kevent change[2];
    static size_t const size = sizeof(change) / sizeof(change[0]);
    EV_SET(&change[0], fd, EVFILT_READ, EV_ADD | (events & READ ? EV_ENABLE : EV_DISABLE), 0, 0, NULL);
    EV_SET(&change[1], fd, EVFILT_WRITE, EV_ADD | (events & WRITE ? EV_ENABLE : EV_DISABLE) , 0, 0, NULL);
    if (unlikely(kevent(_kq, change, size, NULL, 0, NULL) == -1))
      throw OSError(errno, "kqueue");
  } catch (...) {
    remove(fd);
    throw;
  }
}

void KqueueSelector::remove(int fd) {
  Selector::remove(fd);
  try {
    struct kevent change[2];
    static size_t const size = sizeof(change) / sizeof(change[0]);
    EV_SET(&change[0], fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    EV_SET(&change[1], fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    if (unlikely(kevent(_kq, change, size, NULL, 0, NULL) == -1))
      throw OSError(errno, "kqueue");
  } catch (OSError const &e){
    UNUSED(e);
  }
}

void KqueueSelector::modify(int fd, event_t events) {
  Selector::modify(fd, events);
  try {
    struct kevent change[2];
    static size_t const size = sizeof(change) / sizeof(change[0]);
    EV_SET(&change[0], fd, EVFILT_READ, events & READ ? EV_ENABLE : EV_DISABLE, 0, 0, NULL);
    EV_SET(&change[1], fd, EVFILT_WRITE, events & WRITE ? EV_ENABLE : EV_DISABLE, 0, 0, NULL);
    if (unlikely(kevent(_kq, change, size, NULL, 0, NULL) == -1))
      throw OSError(errno, "kqueue");
  } catch (...) {
    Selector::remove(fd);
    throw;
  }
}

void KqueueSelector::select(Events &events, int timeout) const {
  typedef std::vector<struct kevent> KqueueEvents;
  KqueueEvents ev(max_events);
  {
    int nev;
    if (timeout < 0)
      nev = kevent(_kq, NULL, 0, ev.data(), ev.size(), NULL);
    else {
      struct timespec ts;
      ts.tv_sec = timeout / 1000;
      ts.tv_nsec = (timeout % 1000) * 1000000;
      nev = kevent(_kq, NULL, 0, ev.data(), ev.size(), &ts);
    }
    if (nev == -1)
      throw OSError(errno, "kevent");
    ev.resize(nev);
  }
  for (KqueueEvents::const_iterator it = ev.begin(); it != ev.end(); ++it) {
    event_details tmp;
    tmp.fd = it->ident;
    tmp.events = 0;
    if (it->filter == EVFILT_READ)
      tmp.events |= READ;
    if (it->filter == EVFILT_WRITE)
      tmp.events |= WRITE;
    if (it->flags & EV_ERROR)
      tmp.events |= EXCEPT;
    if (tmp.events)
      events.push(tmp);
  }
}

} // namespace ftpp

#endif
