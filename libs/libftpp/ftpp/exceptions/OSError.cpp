/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OSError.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:37:23 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:55:34 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/exceptions/OSError.hpp>

#include <cstring>
#include <string>

namespace ftpp {

OSError::OSError(int __errno, std::string const &s)
    : _errno(__errno), _s(s + ": " + ::strerror(_errno)) {
}

OSError::OSError(OSError const &rhs) : _errno(rhs._errno), _s(rhs._s) {
}

OSError::~OSError() throw() {
}

int OSError::get_errno() const {
  return _errno;
}

OSError &OSError::operator=(OSError const &rhs) {
  if (this != &rhs) {
    _errno = rhs._errno;
    _s = rhs._s;
  }
  return *this;
}

char const *OSError::what() const throw() {
  return _s.c_str();
}

} // namespace ftpp
