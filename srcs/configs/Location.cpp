/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 02:18:19 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/03 02:18:20 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configs/Location.hpp"
#include "configs/Locations/LocationDefault.hpp"
#include "configs/Locations/LocationRedirect.hpp"
#include "configs/Locations/LocationUpload.hpp"

#include <Any.hpp>
#include <Json.hpp>

#include <stdexcept>

Location::Location() {
}

Location::Location(ftpp::Any const &value) {
  ftjson::Object const &location = value.as<ftjson::Object>();
  _takePath(location);
  _takeAllowMethods(location);
}

Location::Location(Location const &rhs)
    : _path(rhs._path), _allow_methods(rhs._allow_methods) {
}

Location &Location::operator=(Location const &rhs) {
  if (this != &rhs) {
    _path = rhs._path;
    _allow_methods = rhs._allow_methods;
  }
  return *this;
}

Location *Location::create(ftpp::Any const &value) {
  Location *result;
  ftjson::Object const &location = value.as<ftjson::Object>();
  ftjson::Object::const_iterator it = location.find("type");
  if (it != location.end()) {
    std::string type = it->second.as<ftjson::String>();
    if (type == "default")
      result = new LocationDefault(value);
    else if (type == "upload")
      result = new LocationUpload(value);
    else if (type == "redirect")
      result = new LocationRedirect(value);
    else
      throw std::runtime_error("Unknown location type");
  } else
    result = new LocationDefault(location);
  return result;
}

Location::~Location() {
}

void Location::_takePath(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("path");
  if (it == location.end())
    throw std::runtime_error("Location without path");
  _path = it->second.as<ftjson::String>();
}

void Location::_takeAllowMethods(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("allow_methods");
  if (it != location.end()) {
    ftjson::Array const &methods = it->second.as<ftjson::Array>();
    for (ftjson::Array::const_iterator it = methods.begin();
         it != methods.end(); ++it)
      _allow_methods.insert(it->as<ftjson::String>());
  }
}

std::string const &Location::getPath() const {
  return _path;
}

Location::AllowMethods const &Location::getAllowMethods() const {
  return _allow_methods;
}
