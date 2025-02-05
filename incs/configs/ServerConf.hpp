/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConf.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:07:45 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/05 02:33:24 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/Location.hpp"
#include "structs/address.hpp"

#include <Any.hpp>
#include <Json.hpp>

#include <list>
#include <map>
#include <set>
#include <string>

class ServerConf {
public:
  static std::size_t const default_client_max_body_size;

  typedef std::set<std::string> ServerNames;
  typedef std::set<address> Addresses;
  typedef std::map<int, std::string> ErrorPages;
  typedef std::list<Location> Locations;

private:
  ServerNames _server_names;
  Addresses _addresses;
  std::size_t _client_max_body_size;
  ErrorPages _error_pages;
  Locations _locations;

  void _takeServerNames(ftjson::Object const &server);
  void _takeAddresses(ftjson::Object const &server);
  void _takeClientBodySize(ftjson::Object const &server);
  void _takeErrorPages(ftjson::Object const &server);
  void _takeLocations(ftjson::Object const &server);

public:
  ServerConf();
  ServerConf(ftpp::Any const &value);
  ServerConf(ServerConf const &rhs);
  ServerConf &operator=(ServerConf const &rhs);
  ~ServerConf();
  void swap(ServerConf &rhs) throw();

  ServerNames const &getServerNames() const;
  Addresses const &getAddresses() const;
  std::size_t getClientMaxBodySize() const;
  ErrorPages const &getErrorPages() const;
  Locations const &getLocations() const;

  Locations::const_iterator findLocation(std::string const &method,
                                         std::string const &path) const;
};
