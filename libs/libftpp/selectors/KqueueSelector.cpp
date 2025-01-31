/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KqueueSelector.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:35:30 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/03 21:32:31 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exceptions/OSError.hpp>
#include <selectors/KqueueSelector.hpp>
#include <macros.hpp>

#if defined(__APPLE__) || defined(__FreeBSD__)

#include <stdexcept>
#include <sys/event.h>
#include <unistd.h>

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
  throw std::logic_error("KqueueSelector::add not implemented");
}

void KqueueSelector::remove(int fd) {
  throw std::logic_error("KqueueSelector::remove not implemented");
}

void KqueueSelector::modify(int fd, event_t events) {
  throw std::logic_error("KqueueSelector::modify not implemented");
}

void KqueueSelector::select(Events &events, int timeout) const {
  throw std::logic_error("KqueueSelector::select not implemented");
}

} // namespace ftpp

#endif
