/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:07:45 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 08:12:05 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/Location.hpp"
#include "structs/Address.hpp"

#include <ftpp/wrapper/Wrapper.hpp>
#include <ftjson/Json.hpp>

#include <map>
#include <set>
#include <string>

class ServerConf {
public:
  static std::size_t const defaultClientMaxBodySize;

  typedef std::set<std::string> ServerNames;
  typedef std::set<Address> Addresses;
  typedef std::map<int, std::string> ErrorPages;
  typedef std::map<std::string, ftpp::Wrapper<Location> > Locations;

private:
  ServerNames _serverNames;
  Addresses _addresses;
  ErrorPages _errorPages;
  std::size_t _clientMaxBodySize;
  Locations _locations;

  void _takeServerNames(ftjson::Object const &server);
  void _takeAddresses(ftjson::Object const &server);
  void _takeErrorPages(ftjson::Object const &server);
  void _takeClientMaxBodySize(ftjson::Object const &server);
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
  ErrorPages const &getErrorPages() const;
  std::size_t getClientMaxBodySize() const;
  Locations const &getLocations() const;

  Locations::const_iterator findLocation(std::string const &path) const;
};
