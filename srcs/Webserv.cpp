/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 07:59:54 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/02 22:56:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include <iostream>

Webserv::Webserv(ftev::EventLoop &loop, Configs const &configs)
    : _configs(configs), _stopper(loop) {
  try {
    Configs::Addresses tmp(_configs.getAllAddresses());
    for (Configs::Addresses::iterator it = tmp.begin(); it != tmp.end(); ++it) {
      HttpServer *server = new HttpServer(loop, it->host, it->port, _configs);
      try {
        _servers.push_back(server);
      } catch (...) {
        delete server;
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
