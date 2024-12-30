/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:07:45 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/30 18:37:51 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Locations/BaseLocation.hpp"

#include <map>
#include <set>
#include <string>

class Server {
public:
  struct Address {
    std::string host;
    int port;
  };

  typedef std::set<std::string> ServerNames;
  typedef std::set<Address> Addresses;
  typedef std::map<int, std::string> ErrorPages;
  typedef std::map<std::string, BaseLocation *> Locations;

  static std::size_t const default_client_max_body_size;
  static Address parseAddress(std::string const &address);
  static std::size_t parseSize(std::string const &str);

private:
  ServerNames _server_names;
  Addresses _addresses;
  std::size_t _client_max_body_size;
  ErrorPages _error_pages;
  Locations _locations;

  static ServerNames _takeServerNames(ftpp::Any const &value);
  static Addresses _takeAddresses(ftpp::Any const &value);
  static std::size_t _takeClientBodySize(ftpp::Any const &value);
  static ErrorPages _takeErrorPages(ftpp::Any const &value);
  static Locations _takeLocations(ftpp::Any const &value);

public:
  Server();
  Server(ftpp::Any const &value);
  Server(Server const &rhs);
  Server &operator=(Server const &rhs);
  ~Server();

  ServerNames const &getServerNames() const;
  Addresses const &getAddresses() const;
  std::size_t const &getClientMaxBodySize() const;
  ErrorPages const &getErrorPage() const;
  Locations const &getLocations() const;
};
