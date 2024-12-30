/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:28:11 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/30 18:49:15 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <Json.hpp>

#include <iostream>

std::size_t const Server::default_client_max_body_size = 1 << 17;

Server::Server() : _client_max_body_size(default_client_max_body_size) {
}

Server::Server(ftpp::Any const &value)
    : _server_names(_takeServerNames(value)), _addresses(_takeAddresses(value)),
      _client_max_body_size(_takeClientBodySize(value)),
      _error_pages(_takeErrorPages(value)), _locations(_takeLocations(value)) {
}

Server::Server(Server const &rhs)
    : _server_names(rhs._server_names), _addresses(rhs._addresses),
      _client_max_body_size(rhs._client_max_body_size),
      _error_pages(rhs._error_pages) {
  Locations tmp;
  try {
    for (Locations::const_iterator it = rhs._locations.begin();
         it != rhs._locations.end(); ++it)
      tmp[it->first] = it->second->copy();
  } catch (...) {
    for (Locations::iterator it = tmp.begin(); it != tmp.end(); ++it)
      delete it->second;
    throw;
  }
  _locations.swap(tmp);
}

Server &Server::operator=(Server const &rhs) {
  if (this != &rhs) {
    Locations tmp;
    try {
      for (Locations::const_iterator it = rhs._locations.begin();
           it != rhs._locations.end(); ++it)
        tmp[it->first] = it->second->copy();
    } catch (...) {
      for (Locations::iterator it = tmp.begin(); it != tmp.end(); ++it)
        delete it->second;
      throw;
    }
    _server_names = rhs._server_names;
    _addresses = rhs._addresses;
    _client_max_body_size = rhs._client_max_body_size;
    _error_pages = rhs._error_pages;
    _locations.swap(tmp);
    for (Locations::iterator it = tmp.begin(); it != tmp.end(); ++it)
      delete it->second;
  }
  return *this;
}

Server::~Server() {
  for (Locations::iterator it = _locations.begin(); it != _locations.end();
       ++it)
    delete it->second;
}

Server::ServerNames const &Server::getServerNames() const {
  return _server_names;
}

Server::Addresses const &Server::getAddresses() const {
  return _addresses;
}

std::size_t const &Server::getClientMaxBodySize() const {
  return _client_max_body_size;
}

Server::ErrorPages const &Server::getErrorPage() const {
  return _error_pages;
}

Server::Locations const &Server::getLocations() const {
  return _locations;
}
