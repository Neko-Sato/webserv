/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:28:11 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/06 19:11:02 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configs/ServerConf.hpp"
#include "structs/Address.hpp"
#include "utility.hpp"

#include <ftjson/Json.hpp>
#include <ftpp/algorithm.hpp>
#include <ftpp/string/string.hpp>

#include <iostream>
#include <limits>

std::size_t const ServerConf::default_client_max_body_size = parseSize("1m");

ServerConf::ServerConf() : _client_max_body_size(default_client_max_body_size) {
}

ServerConf::ServerConf(ftjson::Object const &server) {
  _takeServerNames(server);
  _takeAddresses(server);
  _takeClientBodySize(server);
  _takeErrorPages(server);
  _takeLocations(server);
}

ServerConf::ServerConf(ServerConf const &rhs)
    : _server_names(rhs._server_names), _addresses(rhs._addresses),
      _client_max_body_size(rhs._client_max_body_size),
      _error_pages(rhs._error_pages), _locations(rhs._locations) {
}

ServerConf &ServerConf::operator=(ServerConf const &rhs) {
  if (this != &rhs)
    ServerConf(rhs).swap(*this);
  return *this;
}

ServerConf::~ServerConf() {
}

void ServerConf::swap(ServerConf &rhs) throw() {
  _server_names.swap(rhs._server_names);
  _addresses.swap(rhs._addresses);
  std::swap(_client_max_body_size, rhs._client_max_body_size);
  _error_pages.swap(rhs._error_pages);
  _locations.swap(rhs._locations);
}

void ServerConf::_takeServerNames(ftjson::Object const &server) {
  ftjson::Object::const_iterator const &it = server.find("server_names");
  if (it != server.end()) {
    if (!it->second.isType<ftjson::Array>())
      throw std::runtime_error("server_names is not array");
    ftjson::Array const &names = it->second.as_unsafe<ftjson::Array>();
    for (ftjson::Array::const_iterator it = names.begin(); it != names.end();
         ++it) {
      if (!it->isType<ftjson::String>())
        throw std::runtime_error("server_names is not string");
      _server_names.insert(ftpp::tolower(it->as_unsafe<ftjson::String>()));
    }
  }
}

void ServerConf::_takeAddresses(ftjson::Object const &server) {
  ftjson::Object::const_iterator const &it = server.find("listen");
  if (it != server.end()) {
    if (!it->second.isType<ftjson::Array>())
      throw std::runtime_error("listen is not array");
    ftjson::Array const &addrs = it->second.as_unsafe<ftjson::Array>();
    for (ftjson::Array::const_iterator it = addrs.begin(); it != addrs.end();
         ++it) {
      if (!it->isType<ftjson::Object>())
        throw std::runtime_error("listen element is not object");
      _addresses.insert(Address(it->as_unsafe<ftjson::Object>()));
    }
  } else
    _addresses.insert(Address("0.0.0.0", 8080));
}

void ServerConf::_takeClientBodySize(ftjson::Object const &server) {
  ftjson::Object::const_iterator const &it =
      server.find("client_max_body_size");
  if (it != server.end()) {
    if (!it->second.isType<ftjson::String>())
      throw std::runtime_error("client_max_body_size is not string");
    _client_max_body_size = parseSize(it->second.as_unsafe<ftjson::String>());
  } else
    _client_max_body_size = default_client_max_body_size;
}

void ServerConf::_takeErrorPages(ftjson::Object const &server) {
  ftjson::Object::const_iterator const &it = server.find("error_pages");
  if (it != server.end()) {
    if (!it->second.isType<ftjson::Array>())
      throw std::runtime_error("error_pages is not array");
    ftjson::Array const &pages = it->second.as_unsafe<ftjson::Array>();
    for (ftjson::Array::const_iterator it = pages.begin(); it != pages.end();
         ++it) {
      if (!it->isType<ftjson::Object>())
        throw std::runtime_error("error_page is not object");
      ftjson::Object const &page = it->as_unsafe<ftjson::Object>();
      int code;
      {
        ftjson::Object::const_iterator const &jt = page.find("code");
        if (jt == page.end())
          throw std::runtime_error("error_page without code");
        if (!jt->second.isType<ftjson::Number>())
          throw std::runtime_error("error_page code is not number");
        double tmp = jt->second.as_unsafe<ftjson::Number>();
        code = static_cast<int>(tmp);
        if (std::numeric_limits<int>::min() > tmp ||
            std::numeric_limits<int>::max() < tmp || tmp != code)
          throw std::runtime_error("error_page code out of range");
      }
      ftjson::Object::const_iterator const &jt = page.find("path");
      if (jt == page.end())
        throw std::runtime_error("error_page without path");
      if (!jt->second.isType<ftjson::String>())
        throw std::runtime_error("error_page path is not string");
      std::string const &path = jt->second.as_unsafe<ftjson::String>();
      if (path.empty())
        throw std::runtime_error("error_page path is empty");
      _error_pages[code] = path;
    }
  }
}

void ServerConf::_takeLocations(ftjson::Object const &server) {
  ftjson::Object::const_iterator const &it = server.find("locations");
  if (it != server.end()) {
    if (!it->second.isType<ftjson::Array>())
      throw std::runtime_error("locations is not array");
    ftjson::Array const &locs = it->second.as_unsafe<ftjson::Array>();
    for (ftjson::Array::const_iterator it = locs.begin(); it != locs.end();
         ++it) {
      if (!it->isType<ftjson::Object>())
        throw std::runtime_error("location is not object");
      ftjson::Object const &loc = it->as_unsafe<ftjson::Object>();
      ftjson::Object::const_iterator const &jt = loc.find("path");
      if (jt == loc.end())
        throw std::runtime_error("location without path");
      if (!jt->second.isType<ftjson::String>())
        throw std::runtime_error("location path is not string");
      std::string const &path = jt->second.as_unsafe<ftjson::String>();
      if (path.empty())
        throw std::runtime_error("location path is empty");
      Location(loc).swap(_locations[path]);
    }
  }
}

ServerConf::ServerNames const &ServerConf::getServerNames() const {
  return _server_names;
}

ServerConf::Addresses const &ServerConf::getAddresses() const {
  return _addresses;
}

std::size_t ServerConf::getClientMaxBodySize() const {
  return _client_max_body_size;
}

ServerConf::ErrorPages const &ServerConf::getErrorPages() const {
  return _error_pages;
}

ServerConf::Locations const &ServerConf::getLocations() const {
  return _locations;
}

Location const *ServerConf::findLocation(std::string const &method,
                                         std::string const &path) const {
  std::string lowered_method = ftpp::tolower(method);
  Locations::const_iterator res = _locations.end();
  for (Locations::const_iterator it = _locations.lower_bound(path);
       it != _locations.end() && ftpp::starts_with(path, it->first); ++it) {
    Location::AllowMethods const &allowMethods = it->second.getAllowMethods();
    if (allowMethods.empty() ||
        allowMethods.find(lowered_method) != allowMethods.end())
      res = it;
  }
  if (res == _locations.end())
    return NULL;
  return &res->second;
}
