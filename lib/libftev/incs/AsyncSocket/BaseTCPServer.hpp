/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTCPServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 07:11:15 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/06 10:49:25 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <AsyncSocket/BaseAsyncStreamServer.hpp>

#include <list>

namespace ftev {

class BaseTCPServer {
public:
  EventLoop &loop;

private:
  typedef std::list<BaseAsyncStreamServer *> Servers;
  Servers _servers;

  BaseTCPServer(BaseTCPServer const &rhs);
  BaseTCPServer &operator=(BaseTCPServer const &rhs);

protected:
  BaseTCPServer(EventLoop &loop);
  /*
  This setup should be read in the most concrete class.
  Why? It looks like you can't read a virtual function in the constructor of a
  base class.
  */
  void _setup(char const *host, int port);

public:
  virtual ~BaseTCPServer();

  virtual BaseAsyncStreamServer *create_server(int domain, int type,
                                               int protocol) = 0;
  void start();
};

} // namespace ftev
