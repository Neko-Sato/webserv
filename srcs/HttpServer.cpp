/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 22:55:05 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/02 23:43:18 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"
#include "HttpConnection.hpp"

#include <iostream>

HttpServer::HttpServer(ftev::EventLoop &loop, std::string const &host, int port,
                       Configs const &configs)
    : ftev::BaseTCPServer(loop, host, port), _configs(configs) {
  std::cout << "host:" << host << ", port: " << port << std::endl;
  start();
}

HttpServer::~HttpServer() {
}

void HttpServer::on_connect(ftpp::Socket &socket) {
  try {
    new HttpConnection(loop, socket, *this);
  } catch (std::exception const &e) {
    std::cerr << "HttpServer::on_connect: " << e.what() << std::endl;
  }
}
