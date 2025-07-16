/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationStatic.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:38:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 16:10:58 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "locations/LocationStatic.hpp"
#include "ValidationError.hpp"

#include <ftpp/pathlib/pathlib.hpp>

LocationStatic::LocationStatic() : _autoindex(false) {
}

LocationStatic::LocationStatic(ftjson::Object const &location) : Location(location) {
  _takeRoot(location);
  _takeIndex(location);
  _takeAutoindex(location);
} 

LocationStatic::LocationStatic(LocationStatic const &rhs)
    : Location(rhs), _root(rhs._root), _index(rhs._index),
      _autoindex(rhs._autoindex) {
}

LocationStatic &LocationStatic::operator=(LocationStatic const &rhs) {
  if (this != &rhs)
    LocationStatic(rhs).swap(*this);
  return *this;
}

LocationStatic::~LocationStatic() {
}

void LocationStatic::swap(LocationStatic &rhs) throw() {
  Location::swap(rhs);
  _root.swap(rhs._root);
  _index.swap(rhs._index);
  std::swap(_autoindex, rhs._autoindex);
}

LocationStatic *LocationStatic::clone() const {
  return new LocationStatic(*this);
}

void LocationStatic::_takeRoot(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("root");
  if (it == location.end())
    throw ValidationError("root is not found");
  if (!it->second.isType<ftjson::String>())
    throw ValidationError("root is not string");
  std::string const &root = it->second.as_unsafe<ftjson::String>();
  if (root.empty())
    throw ValidationError("root is empty");
  if (!ftpp::isAbsolutePath(root))
    throw ValidationError("root is not absolute path");
  _root = root;
}

void LocationStatic::_takeIndex(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("index");
  if (it != location.end()) {
    if (!it->second.isType<ftjson::Array>())
      throw ValidationError("index is not array");
    ftjson::Array const &indexes = it->second.as_unsafe<ftjson::Array>();
    for (ftjson::Array::const_iterator it = indexes.begin();
         it != indexes.end(); ++it) {
      if (!it->isType<ftjson::String>())
        throw ValidationError("index is not string");
      std::string const &index = it->as_unsafe<ftjson::String>();
      if (index.empty())
        throw ValidationError("index is empty");
      if (index.find('/') != std::string::npos)
        throw ValidationError("index contains a slash");
      _index.push_back(index);
    }
  }
}

void LocationStatic::_takeAutoindex(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("autoindex");
  if (it != location.end()) {
    if (!it->second.isType<ftjson::Boolean>())
      throw ValidationError("autoindex is not boolean");
    _autoindex = it->second.as_unsafe<ftjson::Boolean>();
  } else
    _autoindex = false;
}

std::string const &LocationStatic::getRoot() const {
  return _root;
}

LocationStatic::IndexFiles const &LocationStatic::getIndex() const {
  return _index;
}

bool LocationStatic::getAutoindex() const {
  return _autoindex;
}
