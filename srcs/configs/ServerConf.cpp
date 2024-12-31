/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:28:11 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 00:38:45 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configs/ServerConf.hpp"
#include "structs/address.hpp"
#include "utility.hpp"

#include <Json.hpp>

#include <iostream>
#include <limits>

std::size_t const ServerConf::default_client_max_body_size = 1 << 20;

ServerConf::ServerConf() : _client_max_body_size(default_client_max_body_size) {
}

ServerConf::ServerConf(ftpp::Any const &value) {
  ftjson::Object const &server = value.as<ftjson::Object>();
  _takeServerNames(server);
  _takeAddresses(server);
  _takeClientBodySize(server);
  _takeErrorPages(server);
  _takeLocations(server);
}

ServerConf::ServerConf(ServerConf const &rhs)
    : _server_names(rhs._server_names), _addresses(rhs._addresses),
      _client_max_body_size(rhs._client_max_body_size),
      _error_pages(rhs._error_pages) {
  Locations tmp;
  try {
    for (Locations::const_iterator it = rhs._locations.begin();
         it != rhs._locations.end(); ++it)
      tmp.push_back((*it)->copy());
  } catch (...) {
    for (Locations::iterator it = tmp.begin(); it != tmp.end();
         it = tmp.erase(it))
      delete *it;
    throw;
  }
  _locations.swap(tmp);
}

ServerConf &ServerConf::operator=(ServerConf const &rhs) {
  if (this != &rhs) {
    Locations tmp;
    try {
      for (Locations::const_iterator it = rhs._locations.begin();
           it != rhs._locations.end(); ++it)
        tmp.push_back((*it)->copy());
    } catch (...) {
      for (Locations::iterator it = tmp.begin(); it != tmp.end();
           it = tmp.erase(it))
        delete *it;
      throw;
    }
    _server_names = rhs._server_names;
    _addresses = rhs._addresses;
    _client_max_body_size = rhs._client_max_body_size;
    _error_pages = rhs._error_pages;
    _locations.swap(tmp);
    for (Locations::iterator it = tmp.begin(); it != tmp.end();
         it = tmp.erase(it))
      delete *it;
  }
  return *this;
}

ServerConf::~ServerConf() {
  for (Locations::iterator it = _locations.begin(); it != _locations.end();
       it = _locations.erase(it))
    delete *it;
}

void ServerConf::_takeServerNames(ftjson::Object const &server) {
  ftjson::Object::const_iterator const &it = server.find("server_names");
  if (it != server.end()) {
    ftjson::Array const &names = it->second.as<ftjson::Array>();
    for (ftjson::Array::const_iterator it = names.begin(); it != names.end();
         ++it)
      _server_names.insert(it->as<ftjson::String>());
  }
}

void ServerConf::_takeAddresses(ftjson::Object const &server) {
  ftjson::Object::const_iterator const &it = server.find("listen");
  if (it != server.end()) {
    ftjson::Array const &addrs = it->second.as<ftjson::Array>();
    for (ftjson::Array::const_iterator it = addrs.begin(); it != addrs.end();
         ++it) {
      ftjson::Object const &addr = it->as<ftjson::Object>();
      std::string host;
      int port;
      ftjson::Object::const_iterator const &jt = addr.find("host");
      if (jt != addr.end())
        host = jt->second.as<ftjson::String>();
      ftjson::Object::const_iterator const &kt = addr.find("port");
      if (kt != addr.end()) {
        double tmp = kt->second.as<ftjson::Number>();
        port = static_cast<int>(tmp);
        if (std::numeric_limits<int>::min() > tmp ||
            std::numeric_limits<int>::max() < tmp || port != tmp)
          throw std::runtime_error("port out of range");
      } else
        throw std::runtime_error("listen without port");
      _addresses.insert(address(host, port));
    }
  }
}

void ServerConf::_takeClientBodySize(ftjson::Object const &server) {
  ftjson::Object::const_iterator const &it =
      server.find("client_max_body_size");
  if (it != server.end())
    _client_max_body_size = parseSize(it->second.as<ftjson::String>());
  else
    _client_max_body_size = default_client_max_body_size;
}

void ServerConf::_takeErrorPages(ftjson::Object const &server) {
  ftjson::Object::const_iterator const &it = server.find("error_pages");
  if (it != server.end()) {
    ftjson::Array const &pages = it->second.as<ftjson::Array>();
    for (ftjson::Array::const_iterator it = pages.begin(); it != pages.end();
         ++it) {
      ftjson::Object const &page = it->as<ftjson::Object>();
      int code;
      {
        ftjson::Object::const_iterator const &jt = page.find("code");
        if (jt == page.end())
          throw std::runtime_error("error_page without code");
        double tmp = jt->second.as<ftjson::Number>();
        code = static_cast<int>(tmp);
        if (std::numeric_limits<int>::min() > tmp ||
            std::numeric_limits<int>::max() < tmp || tmp != code)
          throw std::runtime_error("error_page code out of range");
      }
      ftjson::Object::const_iterator const &jt = page.find("path");
      if (jt == page.end())
        throw std::runtime_error("error_page without path");
      _error_pages[code] = jt->second.as<ftjson::String>();
    }
  }
}

void ServerConf::_takeLocations(ftjson::Object const &server) {
  ftjson::Object::const_iterator const &it = server.find("locations");
  if (it != server.end()) {
    ftjson::Array const &locs = it->second.as<ftjson::Array>();
    try {
      for (ftjson::Array::const_iterator it = locs.begin(); it != locs.end();
           ++it)
        _locations.push_back(Location::create(*it));
    } catch (...) {
      for (Locations::iterator it = _locations.begin(); it != _locations.end();
           it = _locations.erase(it))
        delete *it;
      throw;
    }
  }
}

ServerConf::ServerNames const &ServerConf::getServerNames() const {
  return _server_names;
}

ServerConf::Addresses const &ServerConf::getAddresses() const {
  return _addresses;
}

std::size_t const &ServerConf::getClientMaxBodySize() const {
  return _client_max_body_size;
}

ServerConf::ErrorPages const &ServerConf::getErrorPage() const {
  return _error_pages;
}

ServerConf::Locations const &ServerConf::getLocations() const {
  return _locations;
}
