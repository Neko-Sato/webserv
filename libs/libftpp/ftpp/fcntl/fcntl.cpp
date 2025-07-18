/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fcntl.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:48:38 by uakizuki          #+#    #+#             */
/*   Updated: 2025/07/18 12:33:15 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/macros.hpp>

#include <fcntl.h>
#include <unistd.h>

namespace ftpp {

void setblocking(int fd, bool blocking) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (unlikely(flags == -1))
    throw ftpp::OSError(errno, "fcntl");
  if ((!!(flags & O_NONBLOCK)) ^ !blocking)
    if (unlikely(fcntl(fd, F_SETFL, flags ^ O_NONBLOCK) == -1))
      throw ftpp::OSError(errno, "fcntl");
}

void setcloexec(int fd, bool cloexec) {
  int flags = fcntl(fd, F_GETFD, 0);
  if (unlikely(flags == -1))
    throw ftpp::OSError(errno, "fcntl");
  if ((!!(flags & FD_CLOEXEC)) ^ !!cloexec)
    if (unlikely(fcntl(fd, F_SETFD, flags ^ FD_CLOEXEC) == -1))
      throw ftpp::OSError(errno, "fcntl");
}

} // namespace ftpp
