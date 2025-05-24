/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 02:18:19 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/25 04:40:07 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configs/Location.hpp"
#include "ValidationError.hpp"
#include "locations/LocationDefault.hpp"
#include "locations/LocationRedirect.hpp"
#include "locations/LocationUpload.hpp"

#include <ftjson/Json.hpp>
#include <ftpp/any/Any.hpp>
#include <ftpp/macros.hpp>
#include <ftpp/string/string.hpp>

#include <stdexcept>

template <typename T>
static Location::Detail *_createLocationDetail(ftjson::Object const &location) {
  return new T(location);
}

static Location::Detail::Factories _initFactories() {
  typedef Location::Detail::Factories Factories;
  Factories factories;
  factories["default"] = &_createLocationDetail<LocationDefault>;
  factories["upload"] = &_createLocationDetail<LocationUpload>;
  factories["redirect"] = &_createLocationDetail<LocationRedirect>;
  return factories;
}

Location::Detail::Factories Location::Detail::factories = _initFactories();

Location::Detail::Detail() {
}

Location::Detail::Detail(Detail const &rhs) {
  UNUSED(rhs);
}

Location::Detail::Detail(ftjson::Object const &location) {
  UNUSED(location);
}

Location::Detail &Location::Detail::operator=(Detail const &rhs) {
  UNUSED(rhs);
  return *this;
}

Location::Detail::~Detail() {
}

Location::Location() : _detail(NULL) {
}

Location::Location(ftjson::Object const &location) {
  _takeAllowMethods(location);
  _takeDetail(location);
}

Location::Location(Location const &rhs)
    : _allowMethods(rhs._allowMethods),
      _detail(rhs._detail ? rhs._detail->clone() : NULL) {
}

Location &Location::operator=(Location const &rhs) {
  if (this != &rhs)
    Location(rhs).swap(*this);
  return *this;
}

Location::~Location() {
}

void Location::swap(Location &rhs) throw() {
  _allowMethods.swap(rhs._allowMethods);
  _detail.swap(rhs._detail);
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

void Location::_takeDetail(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("type");
  if (it == location.end())
    throw ValidationError("Location without type");
  if (!it->second.isType<ftjson::String>())
    throw ValidationError("Location type is not string");
  std::string const &type = it->second.as_unsafe<ftjson::String>();
  Detail::Factories::const_iterator factory = Detail::factories.find(type);
  if (factory == Detail::factories.end())
    throw ValidationError("Unknown location type: " + type);
  ftpp::ScopedPtr<Detail>(factory->second(location)).swap(_detail);
}

Location::AllowMethods const &Location::getAllowMethods() const {
  return _allowMethods;
}

Location::Detail const &Location::getDetail() const {
  return *_detail;
}
