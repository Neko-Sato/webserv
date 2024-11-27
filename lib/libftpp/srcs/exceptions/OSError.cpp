/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OSError.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:37:23 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/24 04:07:17 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exceptions/OSError.hpp>

#include <cstring>
#include <string>

namespace ftpp {

OSError::OSError(int __errno, std::string const &s)
    : _errno(__errno), _s(s + ": " + std::strerror(_errno)) {
}

OSError::~OSError() throw() {
}

int OSError::get_errno() const {
  return _errno;
}

char const *OSError::what() const throw() {
  return _s.c_str();
}

} // namespace ftpp
