/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 07:59:54 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/02 09:22:06 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include <iostream>

Webserv::Webserv(ftev::EventLoop &loop, Configs const &configs)
    : _configs(configs), _stopper(loop) {
  try {
    std::set<Address> addresses;
    _configs.getAddressAll().swap(addresses);
    for (std::set<Address>::const_iterator it = addresses.begin();
         it != addresses.end(); ++it) {
      Server *tmp = new Server(loop, it->host, it->port, _configs);
      try {
        _servers.push_back(tmp);
      } catch (...) {
        delete tmp;
        throw;
      }
    }
  } catch (...) {
    for (Servers::iterator it = _servers.begin(); it != _servers.end(); ++it)
      delete *it;
    throw;
  }
}

Webserv::~Webserv() {
  for (Servers::iterator it = _servers.begin(); it != _servers.end(); ++it)
    delete *it;
}
