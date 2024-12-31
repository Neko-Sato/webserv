/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationDefault.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:21:52 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 00:40:47 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configs/Locations/LocationDefault.hpp"

#include <stdexcept>

LocationDefault::LocationDefault(ftpp::Any const &value) : Location(value) {
  ftjson::Object const &location = value.as<ftjson::Object>();
  _takeRoot(location);
  _takeIndex(location);
  _takeAutoindex(location);
  _takeCgi(location);
}

LocationDefault::LocationDefault(LocationDefault const &rhs)
    : Location(rhs), _root(rhs._root), _index(rhs._index),
      _autoindex(rhs._autoindex), _cgi(rhs._cgi) {
}

LocationDefault &LocationDefault::operator=(LocationDefault const &rhs) {
  if (this != &rhs) {
    Location::operator=(rhs);
    _root = rhs._root;
    _index = rhs._index;
    _autoindex = rhs._autoindex;
    _cgi = rhs._cgi;
  }
  return *this;
}

LocationDefault::~LocationDefault() {
}

void LocationDefault::_takeRoot(ftjson::Object const &location) {
  ftjson::Object::const_iterator const &it = location.find("root");
  if (it != location.end())
    _root = it->second.as<ftjson::String>();
  else
    throw std::runtime_error("LocationDefault: root is required");
}

void LocationDefault::_takeIndex(ftjson::Object const &location) {
  ftjson::Object::const_iterator const &it = location.find("index");
  if (it != location.end()) {
    ftjson::Array const &indexes = it->second.as<ftjson::Array>();
    for (ftjson::Array::const_iterator it = indexes.begin();
         it != indexes.end(); ++it)
      _index.push_back(it->as<ftjson::String>());
  }
}

void LocationDefault::_takeAutoindex(ftjson::Object const &location) {
  ftjson::Object::const_iterator const &it = location.find("autoindex");
  if (it != location.end())
    _autoindex = it->second.as<ftjson::Boolean>();
  else
    _autoindex = false;
}

void LocationDefault::_takeCgi(ftjson::Object const &location) {
  ftjson::Object::const_iterator const &it = location.find("cgi");
  if (it != location.end()) {
    ftjson::Object const &cgis = it->second.as<ftjson::Object>();
    for (ftjson::Object::const_iterator it = cgis.begin(); it != cgis.end();
         ++it) {
      std::string const &extension = it->first;
      ftjson::Object const &cgi = it->second.as<ftjson::Object>();
      Cgi tmp;
      {
        ftjson::Object::const_iterator const &it = cgi.find("bin");
        if (it == cgi.end())
          throw std::runtime_error("LocationDefault: cgi without bin");
        tmp.path = it->second.as<ftjson::String>();
      }
      _cgi[extension] = tmp;
    }
  }
}

LocationDefault *LocationDefault::copy() const {
  return new LocationDefault(*this);
}
