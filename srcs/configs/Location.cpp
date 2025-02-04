/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 02:18:19 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/05 01:37:19 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configs/Location.hpp"

#include <Any.hpp>
#include <Json.hpp>
#include <ft_algorithm.hpp>
#include <ft_string.hpp>
#include <macros.hpp>

#include <stdexcept>

Location::Detail::Factories Location::Detail::factories;

Location::Detail::Detail() {
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

Location::Location(ftpp::Any const &value) {
  if (!value.isType<ftjson::Object>())
    throw std::runtime_error("Location is not an object");
  ftjson::Object const &location = value.as_unsafe<ftjson::Object>();
  _takePath(location);
  _takeAllowMethods(location);
  _takeDetail(location);
}

Location::Location(Location const &rhs)
    : _path(rhs._path), _allow_methods(rhs._allow_methods) {
  _detail = rhs._detail ? rhs._detail->clone() : NULL;
}

Location &Location::operator=(Location const &rhs) {
  if (this != &rhs)
    Location(rhs).swap(*this);
  return *this;
}

Location::~Location() {
  delete _detail;
}

void Location::swap(Location &rhs) {
  _path.swap(rhs._path);
  _allow_methods.swap(rhs._allow_methods);
  std::swap(_detail, rhs._detail);
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
      _allow_methods.insert(ftpp::tolower(it->as_unsafe<ftjson::String>()));
    }
  }
}

void Location::_takeDetail(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("type");
  if (it == location.end())
    throw std::runtime_error("Location without type");
  if (!it->second.isType<ftjson::String>())
    throw std::runtime_error("Location type is not string");
  std::string const &type = it->second.as_unsafe<ftjson::String>();
  Detail::Factories::const_iterator factory = Detail::factories.find(type);
  if (factory == Detail::factories.end())
    throw std::runtime_error("Unknown location type: " + type);
  _detail = factory->second(location);
}

std::string const &Location::getPath() const {
  return _path;
}

Location::AllowMethods const &Location::getAllowMethods() const {
  return _allow_methods;
}

Location::Detail const &Location::getDetail() const {
  return *_detail;
}

bool Location::match(std::string const &method, std::string const &path) const {
  if (_allow_methods.empty() ||
      _allow_methods.find(ftpp::tolower(method)) != _allow_methods.end())
    return ftpp::starts_with(path, _path);
  return false;
}
