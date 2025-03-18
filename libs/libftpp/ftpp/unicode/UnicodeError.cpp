/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnicodeError.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 22:04:58 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:55:34 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/unicode/UnicodeError.hpp>

namespace ftpp {

UnicodeError::UnicodeError(std::string const &s)
    : _s(std::string("UnicodeError: ") + s) {
}

UnicodeError::UnicodeError(UnicodeError const &rhs) : _s(rhs._s) {
}

UnicodeError::~UnicodeError() throw() {
}

UnicodeError &UnicodeError::operator=(UnicodeError const &rhs) {
  if (this != &rhs) {
    _s = rhs._s;
  }
  return *this;
}

char const *UnicodeError::what() const throw() {
  return _s.c_str();
}

} // namespace ftpp
