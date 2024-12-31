/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 07:59:54 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/31 10:43:07 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include <iostream>

Webserv::Webserv(ftev::EventLoop &loop, Configs const &configs)
    : _configs(configs), _sigint_handler(loop) {
  try {
    Configs::Addresses tmp(_configs.getAllAddresses());
    for (Configs::Addresses::iterator it = tmp.begin(); it != tmp.end(); ++it)
      std::cout << "host: " << it->host << ", port: " << it->port << std::endl;
  } catch (...) {
    for (Servers::iterator it = _servers.begin(); it != _servers.end(); ++it)
      delete *it;
    throw;
  }
}

Webserv::~Webserv() {
}
