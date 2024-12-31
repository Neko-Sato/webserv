/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationRedirect.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:30:30 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 00:30:34 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configs/Locations/LocationRedirect.hpp"

#include <limits>
#include <stdexcept>

LocationRedirect::LocationRedirect(ftpp::Any const &value) : Location(value) {
  ftjson::Object const &location = value.as<ftjson::Object>();
  _takeCode(location);
  _takeRedirect(location);
}

LocationRedirect::LocationRedirect(LocationRedirect const &rhs)
    : Location(rhs), _code(rhs._code), _redirect(rhs._redirect) {
}

LocationRedirect &LocationRedirect::operator=(LocationRedirect const &rhs) {
  if (this != &rhs) {
    Location::operator=(rhs);
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

void LocationRedirect::_takeCode(ftjson::Object const &location) {
  ftjson::Object::const_iterator const &it = location.find("code");
  if (it != location.end()) {
    double tmp = it->second.as<ftjson::Number>();
    _code = static_cast<int>(tmp);
    if (std::numeric_limits<int>::min() > tmp ||
        std::numeric_limits<int>::max() < tmp || tmp != _code)
      throw std::runtime_error("LocationRedirect: code is out of range");
    if (_code < 300 || 400 <= _code)
      throw std::runtime_error("LocationRedirect: code must be 3xx");
  } else
    throw std::runtime_error("LocationRedirect: code is required");
}

void LocationRedirect::_takeRedirect(ftjson::Object const &location) {
  ftjson::Object::const_iterator const &it = location.find("redirect");
  if (it != location.end())
    _redirect = it->second.as<ftjson::String>();
  else
    throw std::runtime_error("LocationRedirect: redirect is required");
}
