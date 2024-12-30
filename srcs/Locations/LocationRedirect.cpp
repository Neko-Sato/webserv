/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationRedirect.cpp                                :+:      :+:    :+: */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:21:52 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/30 18:06:01 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Locations/LocationRedirect.hpp"

#include <stdexcept>

LocationRedirect::LocationRedirect(ftpp::Any const &value)
    : BaseLocation(value), _code(_takeCode(value)),
      _redirect(_takeRedirect(value)) {
}

LocationRedirect::LocationRedirect(LocationRedirect const &rhs)
    : BaseLocation(rhs), _code(rhs._code), _redirect(rhs._redirect) {
}

LocationRedirect &LocationRedirect::operator=(LocationRedirect const &rhs) {
  if (this != &rhs) {
    BaseLocation::operator=(rhs);
    _code = rhs._code;
    _redirect = rhs._redirect;
  }
  return *this;
}

LocationRedirect::~LocationRedirect() {
}

LocationRedirect *LocationRedirect::copy() const {
  return new LocationRedirect(*this);
}
