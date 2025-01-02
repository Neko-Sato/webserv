/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTCPServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 20:31:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/02 23:14:10 by hshimizu         ###   ########.fr       */
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

    void bind(sockaddr const *addr, socklen_t addrlen);
    void listen(int backlog = 1024);

    void on_read();
    void on_write();
    void on_except();
  };

  std::string _host;
  int _port;

  typedef std::list<Server *> Servers;
  Servers _servers;

  BaseTCPServer();
  BaseTCPServer(BaseTCPServer const &rhs);
  BaseTCPServer &operator=(BaseTCPServer const &rhs);

public:
  BaseTCPServer(EventLoop &loop, std::string const &host, int port);
  virtual ~BaseTCPServer();

  std::string const &getHost() const;
  int getPort() const;

  void start();
  virtual void on_connect(ftpp::Socket &conn) = 0;
};

} // namespace ftev
