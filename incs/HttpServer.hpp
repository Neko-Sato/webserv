/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 22:55:05 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/04 23:08:13 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/Configs.hpp"

#include <AsyncSocket/BaseTCPServer.hpp>
#include <socket/Socket.hpp>

class HttpServer : public ftev::BaseTCPServer {
private:
  Configs const &_configs;

public:
  HttpServer(ftev::EventLoop &loop, std::string const &host, int port,
             Configs const &configs);
  ~HttpServer();

  void on_connect(ftpp::Socket &socket);
  Configs const &getConfigs() const;
};
