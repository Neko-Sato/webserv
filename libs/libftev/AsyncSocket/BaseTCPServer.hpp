/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTCPServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 20:31:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/11 19:05:24 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <AsyncSocket/BaseAsyncSocket.hpp>

#include <socket/Socket.hpp>

#include <list>
#include <string>

namespace ftev {

class BaseTCPServer {
public:
  EventLoop &loop;

private:
  class Server : public BaseAsyncSocket {
  private:
    BaseTCPServer &server;

  public:
    Server(BaseTCPServer &server, int domain, int type, int protocol);
    ~Server();

    void listen(int backlog = 1024);

    void on_read();
    void on_write();
    void on_except();
  };

  typedef std::list<Server *> Servers;
  Servers _servers;

  BaseTCPServer();
  BaseTCPServer(BaseTCPServer const &rhs);
  BaseTCPServer &operator=(BaseTCPServer const &rhs);

public:
  BaseTCPServer(EventLoop &loop, std::string const &host, int port);
  virtual ~BaseTCPServer();

  void start();
  virtual void on_connect(ftpp::Socket &conn) = 0;
};

} // namespace ftev
