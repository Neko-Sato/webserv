/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 07:59:54 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 00:55:15 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include <iostream>

Webserv::Webserv(ftev::EventLoop &loop, Configs const &configs)
    : _configs(configs), _stopper(loop) {
  try {
    Configs::Addresses tmp(_configs.getAllAddresses());
    for (Configs::Addresses::iterator it = tmp.begin(); it != tmp.end(); ++it)
      _servers.push_back(
          new HttpServer(loop, it->host.c_str(), it->port, _configs));
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
