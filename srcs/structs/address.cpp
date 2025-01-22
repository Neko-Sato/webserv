/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   address.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 09:45:58 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/23 06:44:19 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs/address.hpp"

#include <Json.hpp>

#include <stdexcept>
#include <string>

address::address(std::string const &host, int port) : host(host), port(port) {
  if (host.empty())
    throw std::runtime_error("empty host");
  if (0 > port || 65535 < port)
    throw std::runtime_error("port out of range");
}

address::address(ftpp::Any const &value) {
  if (!value.isType<ftjson::Object>())
    throw std::runtime_error("address is not object");
  ftjson::Object const &addr = value.as_unsafe<ftjson::Object>();
  {
    ftjson::Object::const_iterator const &it = addr.find("host");
    if (it != addr.end()) {
      if (!it->second.isType<ftjson::String>())
        throw std::runtime_error("host is not string");
      host = it->second.as_unsafe<ftjson::String>();
      if (host.empty())
        throw std::runtime_error("empty host");
    } else
      host = "localhost";
  }
  {
    ftjson::Object::const_iterator const &it = addr.find("port");
    if (it != addr.end()) {
      if (!it->second.isType<ftjson::Number>())
        throw std::runtime_error("port is not number");
      double tmp = it->second.as_unsafe<ftjson::Number>();
      port = static_cast<int>(tmp);
      if (0 > tmp || 65535 < tmp || port != tmp)
        throw std::runtime_error("port out of range");
    } else
      throw std::runtime_error("address without port");
  }
}

bool address::operator<(address const &rhs) const {
  return host < rhs.host || (host == rhs.host && port < rhs.port);
}
