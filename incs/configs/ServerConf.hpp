/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:07:45 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/17 17:51:45 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/Location.hpp"
#include "structs/Address.hpp"

#include <ftjson/Json.hpp>
#include <ftpp/any/Any.hpp>

#include <map>
#include <set>
#include <string>

class ServerConf {
public:
  static std::size_t const defaultClientMaxBodySize;

  typedef std::set<std::string> ServerNames;
  typedef std::set<Address> Addresses;
  typedef std::map<int, std::string> ErrorPages;
  typedef std::map<std::string, Location> Locations;

private:
  ServerNames _serverNames;
  Addresses _addresses;
  std::size_t _clientMaxBodySize;
  ErrorPages _errorPages;
  Locations _locations;

  void _takeServerNames(ftjson::Object const &server);
  void _takeAddresses(ftjson::Object const &server);
  void _takeClientBodySize(ftjson::Object const &server);
  void _takeErrorPages(ftjson::Object const &server);
  void _takeLocations(ftjson::Object const &server);

public:
  ServerConf();
  ServerConf(ftjson::Object const &server);
  ServerConf(ServerConf const &rhs);
  ServerConf &operator=(ServerConf const &rhs);
  ~ServerConf();
  void swap(ServerConf &rhs) throw();

  ServerNames const &getServerNames() const;
  Addresses const &getAddresses() const;
  std::size_t getClientMaxBodySize() const;
  ErrorPages const &getErrorPages() const;
  Locations const &getLocations() const;

  Location const *findLocation(std::string const &method,
                               std::string const &path) const;
};
