/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:40:08 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 14:09:51 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Connection.hpp"

#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>

#include <iostream>

Server::Server(ftev::EventLoop &loop, std::string const &host, int port,
               Configs const &configs)
    : ftev::TCPServer(loop, host, port), _configs(configs),
      _address(host, port) {
  ftpp::logger(ftpp::Logger::INFO,
               ftpp::Format("Server created (host: {}, port: {})") %
                   _address.host % _address.port);
}

Server::~Server() {
  ftpp::logger(ftpp::Logger::INFO,
               ftpp::Format("Server destory (host: {}, port: {})") %
                   _address.host % _address.port);
}

void Server::on_connect(ftpp::Socket &socket) {
  try {
    new Connection(loop, socket, _address, _configs);
  } catch (std::exception const &e) {
    ftpp::logger(ftpp::Logger::ERROR,
                 ftpp::Format("Server (host: {} port: {}): {}") %
                     _address.host % _address.port % e.what());
  }
}
