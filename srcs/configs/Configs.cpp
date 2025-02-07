/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configs.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 08:30:48 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/08 02:09:44 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configs/Configs.hpp"

#include <Json.hpp>
#include <JsonParser.hpp>

#include <fstream>
#include <stdexcept>
#include <string>

Configs::Configs() {
}

Configs::Configs(ftjson::Object const &configs) {
  _takeServers(configs);
}

Configs::Configs(Configs const &rhs) : _servers(rhs._servers) {
}

Configs &Configs::operator=(const Configs &rhs) {
  if (this != &rhs)
    Configs(rhs).swap(*this);
  return *this;
}

Configs::~Configs() {
}

Configs Configs::load(std::string const &filename) {
  std::ifstream ifs(filename.c_str());
  if (!ifs)
    throw std::runtime_error("Could not open configuration file");
  try {
    ftpp::Any configs;
    ftjson::JsonParser::parse(ifs).swap(configs);
    if (!configs.isType<ftjson::Object>())
      throw std::runtime_error("configs is not object");
    return Configs(configs.as_unsafe<ftjson::Object>());
  } catch (std::exception const &e) {
    throw std::runtime_error("Could not parse configuration file: " +
                             std::string(e.what()));
  }
}

void Configs::swap(Configs &rhs) {
  _servers.swap(rhs._servers);
}

void Configs::_takeServers(ftjson::Object const &configs) {
  ftjson::Object::const_iterator it = configs.find("servers");
  if (it != configs.end()) {
    if (!it->second.isType<ftjson::Array>())
      throw std::runtime_error("servers is not array");
    ftjson::Array const &server = it->second.as_unsafe<ftjson::Array>();
    for (ftjson::Array::const_iterator it = server.begin(); it != server.end();
         ++it) {
      if (!it->isType<ftjson::Object>())
        throw std::runtime_error("server is not object");
      _servers.push_back(ServerConf(it->as_unsafe<ftjson::Object>()));
    }
  }
}

Configs::Servers const &Configs::getServers() const {
  return _servers;
}

ServerConf const &Configs::findServer(address const &addr,
                                      std::string const &name) const {
  ServerConf const *first = NULL;
  for (Servers::const_iterator it = _servers.begin(); it != _servers.end();
       ++it) {
    ServerConf::Addresses const &addrs = it->getAddresses();
    if (addrs.find(addr) != addrs.end()) {
      ServerConf::ServerNames const &names = it->getServerNames();
      if (name.empty() || names.find(name) != names.end())
        return *it;
      if (!first)
        first = &*it;
    }
  }
  if (!first)
    throw std::runtime_error("Not found server");
  return *first;
}

std::set<address> Configs::getAddressAll() const {
  std::set<address> addresses;
  for (Servers::const_iterator it = _servers.begin(); it != _servers.end();
       ++it) {
    ServerConf::Addresses const &addrs = it->getAddresses();
    addresses.insert(addrs.begin(), addrs.end());
  }
  return addresses;
}
