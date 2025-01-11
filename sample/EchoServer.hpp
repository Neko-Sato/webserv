/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EchoServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 04:31:15 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/11 23:01:09 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <AsyncSocket/BaseTCPConnection.hpp>
#include <AsyncSocket/BaseTCPServer.hpp>

#include <EventLoop.hpp>

class EchoServer : public ftev::BaseTCPServer {
private:
  class Connection : public ftev::BaseTCPConnection {
  public:
    Connection(EchoServer &server, ftpp::Socket &socket);
    ~Connection();

    void on_data(std::vector<char> const &data);
    void on_eof();
    void on_drain();
    void on_except();
    void on_release();
  };

public:
  EchoServer(ftev::EventLoop &loop, char const *host, int port);
  ~EchoServer();

  void on_connect(ftpp::Socket &socket);
};
