/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:40:37 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/30 02:03:46 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/Configs.hpp"
#include "structs/Address.hpp"

#include <ftev/Stream/TCPServer.hpp>
#include <ftpp/socket/Socket.hpp>

class Server : public ftev::TCPServer {
private:
  Configs const &_configs;
  Address _address;

public:
  Server(ftev::EventLoop &loop, std::string const &host, int port,
         Configs const &configs);
  ~Server();

  void on_connect(ftpp::Socket &socket);
};
