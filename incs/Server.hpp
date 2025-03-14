/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:40:37 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/22 08:48:43 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/Configs.hpp"
#include "structs/Address.hpp"

#include <AsyncSocket/BaseTCPServer.hpp>
#include <socket/Socket.hpp>

class Server : public ftev::BaseTCPServer {
private:
  Configs const &_configs;
  Address _address;
  time_t _timeout;

public:
  Server(ftev::EventLoop &loop, std::string const &host, int port,
         Configs const &configs);
  ~Server();

  void on_connect(ftpp::Socket &socket);
  Configs const &getConfigs() const;
  Address const &getAddress() const;
};
