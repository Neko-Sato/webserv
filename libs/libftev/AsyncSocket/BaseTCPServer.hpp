/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTCPServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 20:31:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/26 12:35:42 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <AsyncSocket/BaseAsyncSocket.hpp>

#include <NonCopyable.hpp>
#include <socket/Socket.hpp>

#include <list>
#include <string>

namespace ftev {

class BaseTCPServer : private ftpp::NonCopyable {
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

public:
  BaseTCPServer(EventLoop &loop, std::string const &host, int port);
  virtual ~BaseTCPServer();

  virtual void on_connect(ftpp::Socket &conn) = 0;
};

} // namespace ftev
