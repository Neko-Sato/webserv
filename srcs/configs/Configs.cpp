/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configs.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 08:30:48 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 00:25:01 by hshimizu         ###   ########.fr       */
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

Configs::Configs(ftpp::Any const &value) {
  ftjson::Object const &configs = value.as<ftjson::Object>();
  _takeServers(configs);
}

Configs::Configs(Configs const &rhs) : _servers(rhs._servers) {
}

Configs &Configs::operator=(const Configs &rhs) {
  if (this != &rhs) {
    _servers = rhs._servers;
  }
  return *this;
}

Configs::~Configs() {
}

Configs Configs::load(std::string const &filename) {
  std::ifstream ifs(filename.c_str());
  if (!ifs)
    throw std::runtime_error("Could not open configuration file");
  try {
    return Configs(ftjson::JsonParser::parse(ifs));
  } catch (std::bad_cast const &) {
    throw std::runtime_error("Bad Configuration file");
  }
}

void Configs::_takeServers(ftjson::Object const &configs) {
  ftjson::Object::const_iterator it = configs.find("servers");
  if (it != configs.end()) {
    ftjson::Array const &server = it->second.as<ftjson::Array>();
    for (ftjson::Array::const_iterator it = server.begin(); it != server.end();
         ++it)
      _servers.push_back(ServerConf(*it));
  }
}

Configs::Addresses Configs::getAllAddresses() const {
  Addresses result;
  for (Servers::const_iterator it = _servers.begin(); it != _servers.end();
       ++it) {
    ServerConf::Addresses const &tmp = it->getAddresses();
    result.insert(tmp.begin(), tmp.end());
  }
  return result;
}
