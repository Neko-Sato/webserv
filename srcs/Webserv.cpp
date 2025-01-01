/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 07:59:54 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 12:35:23 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include <iostream>

Webserv::Webserv(ftev::EventLoop &loop, Configs const &configs)
    : _configs(configs), _stopper(loop) {
  try {
    Configs::Addresses tmp(_configs.getAllAddresses());
    for (Configs::Addresses::iterator it = tmp.begin(); it != tmp.end(); ++it)
      std::cout << "host: " << it->host << ", port: " << it->port << std::endl;
  } catch (...) {

    throw;
  }
}

Webserv::~Webserv() {
}
