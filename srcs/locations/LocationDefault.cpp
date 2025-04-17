/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationDefault.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:38:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/17 20:38:38 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <locations/LocationDefault.hpp>
#include <tasks/DefaultTask.hpp>

#include <stdexcept>

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
    throw std::runtime_error("root is not found");
  if (!it->second.isType<ftjson::String>())
    throw std::runtime_error("root is not string");
  _root = it->second.as_unsafe<std::string>();
}

void LocationDefault::_takeIndex(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("index");
  if (it != location.end()) {
    if (!it->second.isType<ftjson::Array>())
      throw std::runtime_error("index is not array");
    ftjson::Array const &indexes = it->second.as_unsafe<ftjson::Array>();
    for (ftjson::Array::const_iterator it = indexes.begin();
         it != indexes.end(); ++it) {
      if (!it->isType<ftjson::String>())
        throw std::runtime_error("index is not string");
      _index.insert(it->as_unsafe<ftjson::String>());
    }
  }
}

void LocationDefault::_takeAutoindex(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("autoindex");
  if (it != location.end()) {
    if (!it->second.isType<ftjson::Boolean>())
      throw std::runtime_error("autoindex is not boolean");
    _autoindex = it->second.as_unsafe<ftjson::Boolean>();
  } else
    _autoindex = false;
}

void LocationDefault::_takeCgi(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("cgi");
  if (it != location.end()) {
    if (!it->second.isType<ftjson::Array>())
      throw std::runtime_error("cgi is not array");
    ftjson::Array const &cgis = it->second.as_unsafe<ftjson::Array>();
    for (ftjson::Array::const_iterator it = cgis.begin(); it != cgis.end();
         ++it) {
      if (!it->isType<ftjson::Object>())
        throw std::runtime_error("cgi is not object");
      ftjson::Object const &cgi = it->as_unsafe<ftjson::Object>();
      ftjson::Object::const_iterator jt = cgi.find("ext");
      if (jt == cgi.end())
        throw std::runtime_error("cgi without ext");
      if (!jt->second.isType<ftjson::String>())
        throw std::runtime_error("cgi ext is not string");
      std::string const &ext = jt->second.as_unsafe<ftjson::String>();
      if (ext.empty())
        throw std::runtime_error("cgi ext is empty");
      if (ext[0] != '.')
        throw std::runtime_error("cgi ext is not start with dot");
      jt = cgi.find("bin");
      if (jt == cgi.end())
        throw std::runtime_error("cgi without bin");
      if (!jt->second.isType<ftjson::String>())
        throw std::runtime_error("cgi bin is not string");
      std::string const &bin = jt->second.as_unsafe<ftjson::String>();
      if (bin.empty())
        throw std::runtime_error("cgi bin is empty");
      _cgi[ext] = bin;
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

Task *LocationDefault::createTask(ftev::StreamConnectionTransport &transport,
                                  ftev::EventLoop::DeferWatcher &complete,
                                  Request const &request) const {
  return new DefaultTask(transport, complete, request, *this);
}
