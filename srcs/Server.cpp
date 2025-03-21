/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:40:08 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/16 00:21:42 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Connection.hpp"

#include <Format.hpp>
#include <logger/Logger.hpp>

#include <iostream>

Server::Server(ftev::EventLoop &loop, std::string const &host, int port,
               Configs const &configs)
    : ftev::BaseTCPServer(loop, host, port), _configs(configs),
      _address(host, port) {
  ftpp::logger.log(ftpp::Logger::INFO,
                   ftpp::Format("Server created (host: {}, port: {})") %
                       _address.host % _address.port);
}

Server::~Server() {
}

void Server::on_connect(ftpp::Socket &socket) {
  try {
    new Connection(loop, socket, *this);
  } catch (std::exception const &e) {
    std::cerr << "Failed to connect: " << e.what() << std::endl;
  }
}

Configs const &Server::getConfigs() const {
  return _configs;
}

Address const &Server::getAddress() const {
  return _address;
}
