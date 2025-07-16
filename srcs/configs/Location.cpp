/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 02:18:19 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 16:00:07 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configs/Location.hpp"
#include "ValidationError.hpp"
#include "locations/LocationStatic.hpp"
#include "locations/LocationRedirect.hpp"
#include "locations/LocationUpload.hpp"
#include "locations/LocationCgi.hpp"
#include "utility.hpp"

#include <ftjson/Json.hpp>
#include <ftpp/math/math.hpp>
#include <ftpp/macros.hpp>

#include <cassert>
#include <limits>

template <typename T>
static Location *_createLocationDetail(ftjson::Object const &location) {
  return new T(location);
}

static Location::Factories _initFactories() {
  typedef Location::Factories Factories;
  Factories factories;
  factories["static"] = &_createLocationDetail<LocationStatic>;
  factories["cgi"] = &_createLocationDetail<LocationCgi>;
  factories["upload"] = &_createLocationDetail<LocationUpload>;
  factories["redirect"] = &_createLocationDetail<LocationRedirect>;
  return factories;
}

Location::Factories Location::factories = _initFactories();

Location *Location::create(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("type");
  if (it == location.end())
    throw ValidationError("Location without type");
  if (!it->second.isType<ftjson::String>())
    throw ValidationError("Location type is not string");
  std::string const &type = it->second.as_unsafe<ftjson::String>();
  Factories::const_iterator factory = factories.find(type);
  if (factory == factories.end())
    throw ValidationError("Unknown location type: " + type);
  return factory->second(location);
}

Location::Location() {
}

Location::Location(Location const &rhs)
  : _allowMethods(rhs._allowMethods),
    _clientMaxBodySize(rhs._clientMaxBodySize) {
}

Location::Location(ftjson::Object const &location) {
  _takeAllowMethods(location);
  _takeClientMaxBodySize(location);
}

Location &Location::operator=(Location const &rhs) {
  assert(false);
  UNUSED(rhs);
  return *this;
}

Location::~Location() {
}

void Location::swap(Location &rhs) throw() {
  _allowMethods.swap(rhs._allowMethods);
  _clientMaxBodySize.swap(rhs._clientMaxBodySize);
}

void Location::_takeAllowMethods(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("allow_methods");
  if (it != location.end()) {
    if (!it->second.isType<ftjson::Array>())
      throw ValidationError("allow_methods is not array");
    ftjson::Array const &methods = it->second.as_unsafe<ftjson::Array>();
    for (ftjson::Array::const_iterator it = methods.begin();
         it != methods.end(); ++it) {
      if (!it->isType<ftjson::String>())
        throw ValidationError("allow_methods is not string");
      _allowMethods.insert(it->as_unsafe<ftjson::String>());
    }
  }
}

void Location::_takeClientMaxBodySize(ftjson::Object const &location) {
  ftjson::Object::const_iterator const &it = location.find("client_max_body_size");
  if (it != location.end()) {
    if (it->second.isType<ftjson::String>()) {
      std::string const &sizeStr = it->second.as_unsafe<ftjson::String>();
      _clientMaxBodySize = parseSize(sizeStr);
    } else if (it->second.isType<ftjson::Number>()) {
      double tmp = it->second.as_unsafe<ftjson::Number>();
      if (static_cast<double>(std::numeric_limits<std::size_t>::min()) > tmp ||
        static_cast<double>(std::numeric_limits<std::size_t>::max()) < tmp ||
        ftpp::isInteger(tmp))
        throw ValidationError("client_max_body_size out of range");
      _clientMaxBodySize = static_cast<std::size_t>(tmp);
    } else
      throw ValidationError("client_max_body_size is not string or number");
  }
}

Location::AllowMethods const &Location::getAllowMethods() const {
  return _allowMethods;
}

ftpp::Optional<std::size_t> const &Location::getClientMaxBodySize() const {
  return _clientMaxBodySize;
}
