/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 02:18:19 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/23 06:48:13 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configs/Location.hpp"

#include <Any.hpp>
#include <Json.hpp>

#include <stdexcept>

Location::Location() {
}

Location::Location(ftpp::Any const &value) {
  if (!value.isType<ftjson::Object>())
    throw std::runtime_error("Location is not an object");
  ftjson::Object const &location = value.as_unsafe<ftjson::Object>();
  _takePath(location);
  _takeAllowMethods(location);
}

Location::Location(Location const &rhs)
    : _path(rhs._path), _allow_methods(rhs._allow_methods) {
}

Location &Location::operator=(Location const &rhs) {
  if (this != &rhs)
    Location(rhs).swap(*this);
  return *this;
}

Location::~Location() {
}

void Location::swap(Location &rhs) {
  _path.swap(rhs._path);
  _allow_methods.swap(rhs._allow_methods);
}

void Location::_takePath(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("path");
  if (it == location.end())
    throw std::runtime_error("Location without path");
  if (!it->second.isType<ftjson::String>())
    throw std::runtime_error("Location path is not string");
  _path = it->second.as_unsafe<ftjson::String>();
}

void Location::_takeAllowMethods(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("allow_methods");
  if (it != location.end()) {
    if (!it->second.isType<ftjson::Array>())
      throw std::runtime_error("allow_methods is not array");
    ftjson::Array const &methods = it->second.as_unsafe<ftjson::Array>();
    for (ftjson::Array::const_iterator it = methods.begin();
         it != methods.end(); ++it) {
      if (!it->isType<ftjson::String>())
        throw std::runtime_error("allow_methods is not string");
      _allow_methods.insert(it->as_unsafe<ftjson::String>());
    }
  }
}
