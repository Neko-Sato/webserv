/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DevpollSelector.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 00:18:31 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/30 01:47:18 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if defined(__sun__)

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/macros.hpp>
#include <ftpp/selectors/DevpollSelector.hpp>

#include <fcntl.h>
#include <poll.h>
#include <stdexcept>
#include <unistd.h>

namespace ftpp {

int DevpollSelector::_create_devpollfd() {
  int fd = open("/dev/poll", O_RDWR | O_CLOEXEC);
  if (unlikely(fd == -1))
    throw OSError(errno, "open");
  return fd;
}

DevpollSelector::DevpollSelector() {
  _fd = _create_devpollfd();
}

DevpollSelector::~DevpollSelector() {
  close(_fd);
}

void DevpollSelector::add(int fd, event_t events) {
  throw std::logic_error("DevpollSelector::add not implemented");
}

void DevpollSelector::remove(int fd) {
  throw std::logic_error("DevpollSelector::remove not implemented");
}

void DevpollSelector::modify(int fd, event_t events) {
  throw std::logic_error("DevpollSelector::modify not implemented");
}

void DevpollSelector::select(Events &events, int timeout) const {
  throw std::logic_error("DevpollSelector::select not implemented");
}

} // namespace ftpp

#endif
