/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setblocking.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 14:22:47 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/28 14:40:43 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/macros.hpp>

#include <fcntl.h>

namespace ftev {

void setblocking(int fd, bool blocking) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (unlikely(flags == -1))
    throw ftpp::OSError(errno, "fcntl");
  if ((!!(flags & O_NONBLOCK)) ^ !!blocking)
    if (unlikely(fcntl(fd, F_SETFL, flags ^ O_NONBLOCK) == -1))
      throw ftpp::OSError(errno, "fcntl");
}

} // namespace ftev
