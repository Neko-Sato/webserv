/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationDefault.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:38:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/25 04:36:49 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "locations/LocationDefault.hpp"
#include "ValidationError.hpp"
#include "tasks/DefaultTask.hpp"

#include <ftpp/pathlib/pathlib.hpp>

#include <stdexcept>

void LocationDefault::Cgi::swap(Cgi &rhs) throw() {
  bin.swap(rhs.bin);
}

LocationDefault::LocationDefault() : _autoindex(false) {
}

LocationDefault::LocationDefault(ftjson::Object const &location) {
  _takeRoot(location);
  _takeIndex(location);
  _takeAutoindex(location);
  _takeCgi(location);
}

LocationDefault::LocationDefault(LocationDefault const &rhs)
    : Detail(rhs), _root(rhs._root), _index(rhs._index),
      _autoindex(rhs._autoindex), _cgi(rhs._cgi) {
}

LocationDefault &LocationDefault::operator=(LocationDefault const &rhs) {
  if (this != &rhs)
    LocationDefault(rhs).swap(*this);
  return *this;
}

LocationDefault::~LocationDefault() {
}

void LocationDefault::swap(LocationDefault &rhs) throw() {
  _root.swap(rhs._root);
  _index.swap(rhs._index);
  std::swap(_autoindex, rhs._autoindex);
  _cgi.swap(rhs._cgi);
}

LocationDefault *LocationDefault::clone() const {
  return new LocationDefault(*this);
}

void LocationDefault::_takeRoot(ftjson::Object const &location) {
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

void LocationDefault::_takeIndex(ftjson::Object const &location) {
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
      _index.insert(index);
    }
  }
}

void LocationDefault::_takeAutoindex(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("autoindex");
  if (it != location.end()) {
    if (!it->second.isType<ftjson::Boolean>())
      throw ValidationError("autoindex is not boolean");
    _autoindex = it->second.as_unsafe<ftjson::Boolean>();
  } else
    _autoindex = false;
}

void LocationDefault::_takeCgi(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("cgi");
  if (it != location.end()) {
    if (!it->second.isType<ftjson::Array>())
      throw ValidationError("cgi is not array");
    ftjson::Array const &cgis = it->second.as_unsafe<ftjson::Array>();
    for (ftjson::Array::const_iterator it = cgis.begin(); it != cgis.end();
         ++it) {
      if (!it->isType<ftjson::Object>())
        throw ValidationError("cgi is not object");
      ftjson::Object const &cgi = it->as_unsafe<ftjson::Object>();
      ftjson::Object::const_iterator jt = cgi.find("ext");
      if (jt == cgi.end())
        throw ValidationError("cgi without ext");
      if (!jt->second.isType<ftjson::String>())
        throw ValidationError("cgi ext is not string");
      std::string const &ext = jt->second.as_unsafe<ftjson::String>();
      if (ext.empty())
        throw ValidationError("cgi ext is empty");
      if (ext[0] != '.')
        throw ValidationError("cgi ext is not start with dot");
      Cgi tmp;
      jt = cgi.find("bin");
      if (jt == cgi.end())
        throw ValidationError("cgi without bin");
      if (!jt->second.isType<ftjson::String>())
        throw ValidationError("cgi bin is not string");
      std::string const &bin = jt->second.as_unsafe<ftjson::String>();
      if (bin.empty())
        throw ValidationError("cgi bin is empty");
      if (!ftpp::isAbsolutePath(bin))
        throw ValidationError("cgi bin is not absolute path");
      tmp.bin = bin;
      _cgi[ext].swap(tmp);
    }
  }
}

std::string const &LocationDefault::getRoot() const {
  return _root;
}

LocationDefault::Indexes const &LocationDefault::getIndex() const {
  return _index;
}

bool LocationDefault::getAutoindex() const {
  return _autoindex;
}

LocationDefault::Cgis const &LocationDefault::getCgis() const {
  return _cgi;
}

Task *LocationDefault::createTask(Connection::Cycle &cycle, std::string const &path) const {
  return new DefaultTask(cycle, *this, path);
}
