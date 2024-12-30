/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configs.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 08:30:48 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/30 18:38:22 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Configs.hpp"

#include <Json.hpp>
#include <JsonParser.hpp>

#include <fstream>
#include <stdexcept>
#include <string>

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

Configs::Configs() {
}

Configs::Configs(ftpp::Any const &value) : _servers(_takeServers(value)) {
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

Configs::Servers const &Configs::getServers() const {
  return _servers;
}