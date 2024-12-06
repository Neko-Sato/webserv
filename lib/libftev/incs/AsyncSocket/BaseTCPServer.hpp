/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTCPServer.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 07:11:15 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/06 09:32:15 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <AsyncSocket/BaseAsyncStreamServer.hpp>

#include <list>

namespace ftev {

class BaseTCPServer {
private:
  EventLoop &loop;
  typedef std::list<BaseAsyncStreamServer *> Servers;
  Servers _servers;

  void _setup(char const *host, int port);

  BaseTCPServer(BaseTCPServer const &rhs);
  BaseTCPServer &operator=(BaseTCPServer const &rhs);

protected:
  BaseTCPServer(EventLoop &loop, char const *host, int port);

public:
  virtual ~BaseTCPServer();

  virtual BaseAsyncStreamServer *create_server() = 0;
  void start();
};

} // namespace ftev
