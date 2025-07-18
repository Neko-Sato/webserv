/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:28:11 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/18 13:50:49 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configs/ServerConf.hpp"
#include "ValidationError.hpp"
#include "structs/Address.hpp"
#include "utility.hpp"

#include <ftjson/Json.hpp>
#include <ftpp/algorithm.hpp>
#include <ftpp/string/string.hpp>
#include <ftpp/math/math.hpp>

#include <algorithm>
#include <limits>

std::size_t const ServerConf::defaultClientMaxBodySize = parseSize("1m");

ServerConf::ServerConf() {
}

ServerConf::ServerConf(ftjson::Object const &server) {
  _takeServerNames(server);
  _takeAddresses(server);
  _takeErrorPages(server);
  _takeClientMaxBodySize(server);
  _takeLocations(server);
}

ServerConf::ServerConf(ServerConf const &rhs)
    : _serverNames(rhs._serverNames), _addresses(rhs._addresses),
    _errorPages(rhs._errorPages), _clientMaxBodySize(rhs._clientMaxBodySize),
    _locations(rhs._locations) {
}

ServerConf &ServerConf::operator=(ServerConf const &rhs) {
  if (this != &rhs)
    ServerConf(rhs).swap(*this);
  return *this;
}

ServerConf::~ServerConf() {
}

void ServerConf::swap(ServerConf &rhs) throw() {
  _serverNames.swap(rhs._serverNames);
  _addresses.swap(rhs._addresses);
  _errorPages.swap(rhs._errorPages);
  std::swap(_clientMaxBodySize, rhs._clientMaxBodySize);
  _locations.swap(rhs._locations);
}

void ServerConf::_takeServerNames(ftjson::Object const &server) {
  ftjson::Object::const_iterator const &it = server.find("server_names");
  if (it != server.end()) {
    if (!it->second.isType<ftjson::Array>())
      throw ValidationError("server_names is not array");
    ftjson::Array const &names = it->second.as_unsafe<ftjson::Array>();
    for (ftjson::Array::const_iterator it = names.begin(); it != names.end();
         ++it) {
      if (!it->isType<ftjson::String>())
        throw ValidationError("server_names is not string");
      _serverNames.insert(ftpp::tolower(it->as_unsafe<ftjson::String>()));
    }
  }
}

void ServerConf::_takeAddresses(ftjson::Object const &server) {
  ftjson::Object::const_iterator const &it = server.find("listen");
  if (it != server.end()) {
    if (!it->second.isType<ftjson::Array>())
      throw ValidationError("listen is not array");
    ftjson::Array const &addrs = it->second.as_unsafe<ftjson::Array>();
    for (ftjson::Array::const_iterator it = addrs.begin(); it != addrs.end();
         ++it) {
      if (!it->isType<ftjson::Object>())
        throw ValidationError("listen element is not object");
      _addresses.insert(Address(it->as_unsafe<ftjson::Object>()));
    }
  } else
    _addresses.insert(Address("0.0.0.0", 8080));
}

void ServerConf::_takeErrorPages(ftjson::Object const &server) {
  ftjson::Object::const_iterator const &it = server.find("error_pages");
  if (it != server.end()) {
    if (!it->second.isType<ftjson::Array>())
      throw ValidationError("error_pages is not array");
    ftjson::Array const &pages = it->second.as_unsafe<ftjson::Array>();
    for (ftjson::Array::const_iterator it = pages.begin(); it != pages.end();
         ++it) {
      if (!it->isType<ftjson::Object>())
        throw ValidationError("error_page is not object");
      ftjson::Object const &page = it->as_unsafe<ftjson::Object>();
      int code;
      {
        ftjson::Object::const_iterator const &jt = page.find("code");
        if (jt == page.end())
          throw ValidationError("error_page without code");
        if (!jt->second.isType<ftjson::Number>())
          throw ValidationError("error_page code is not number");
        double tmp = jt->second.as_unsafe<ftjson::Number>();
        if (static_cast<double>(std::numeric_limits<int>::min()) > tmp ||
          static_cast<double>(std::numeric_limits<int>::max()) < tmp ||
          !ftpp::isInteger(tmp))
          throw ValidationError("error_page code out of range");
        code = static_cast<int>(tmp);
        }
      ftjson::Object::const_iterator const &jt = page.find("path");
      if (jt == page.end())
        throw ValidationError("error_page without path");
      if (!jt->second.isType<ftjson::String>())
        throw ValidationError("error_page path is not string");
      std::string const &path = jt->second.as_unsafe<ftjson::String>();
      if (path.empty())
        throw ValidationError("error_page path is empty");
      _errorPages[code] = path;
    }
  }
}

void ServerConf::_takeClientMaxBodySize(ftjson::Object const &server) {
  ftjson::Object::const_iterator const &it = server.find("client_max_body_size");
  if (it != server.end()) {
    if (it->second.isType<ftjson::String>()) {
      std::string const &sizeStr = it->second.as_unsafe<ftjson::String>();
      _clientMaxBodySize = parseSize(sizeStr);
    } else if (it->second.isType<ftjson::Number>()) {
      double tmp = it->second.as_unsafe<ftjson::Number>();
      if (static_cast<double>(std::numeric_limits<std::size_t>::min()) > tmp ||
        static_cast<double>(std::numeric_limits<std::size_t>::max()) < tmp ||
        !ftpp::isInteger(tmp))
        throw ValidationError("client_max_body_size out of range");
      _clientMaxBodySize = static_cast<std::size_t>(tmp);
    } else
      throw ValidationError("client_max_body_size is not string or number");
  } else
    _clientMaxBodySize = defaultClientMaxBodySize;
}

void ServerConf::_takeLocations(ftjson::Object const &server) {
  ftjson::Object::const_iterator const &it = server.find("locations");
  if (it != server.end()) {
    if (!it->second.isType<ftjson::Array>())
      throw ValidationError("locations is not array");
    ftjson::Array const &locs = it->second.as_unsafe<ftjson::Array>();
    for (ftjson::Array::const_iterator it = locs.begin(); it != locs.end();
         ++it) {
      if (!it->isType<ftjson::Object>())
        throw ValidationError("location is not object");
      ftjson::Object const &loc = it->as_unsafe<ftjson::Object>();
      ftjson::Object::const_iterator const &jt = loc.find("path");
      if (jt == loc.end())
        throw ValidationError("location without path");
      if (!jt->second.isType<ftjson::String>())
        throw ValidationError("location path is not string");
      std::string const &path = jt->second.as_unsafe<ftjson::String>();
      if (path.empty())
        throw ValidationError("location path is empty");
      _locations[path].reset(Location::create(loc));
    }
  }
}

ServerConf::ServerNames const &ServerConf::getServerNames() const {
  return _serverNames;
}

ServerConf::Addresses const &ServerConf::getAddresses() const {
  return _addresses;
}

ServerConf::ErrorPages const &ServerConf::getErrorPages() const {
  return _errorPages;
}

std::size_t ServerConf::getClientMaxBodySize() const {
  return _clientMaxBodySize;
}

ServerConf::Locations const &ServerConf::getLocations() const {
  return _locations;
}

ServerConf::Locations::const_iterator ServerConf::findLocation(std::string const &path) const {
  for (Locations::const_reverse_iterator it =
           Locations::const_reverse_iterator(_locations.upper_bound(path));
       it != _locations.rend(); ++it) {
    if (!ftpp::starts_with(path, it->first))
      continue;
    if (it->first.size() != path.size() &&
        !(*it->first.rbegin() == '/' || path[it->first.size()] == '/'))
      continue;
    return --it.base();
  }
  return _locations.end();
}
