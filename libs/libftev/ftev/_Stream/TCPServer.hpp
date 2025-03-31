/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 02:03:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/01 01:07:10 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/Stream/StreamServer.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>

#include <list>
#include <string>

namespace ftev {

class TCPServer : private ftpp::NonCopyable {
private:
  class Handler : public StreamServer {
  private:
    TCPServer &_server;

  public:
    Handler(EventLoop &loop, ftpp::Socket &socket, TCPServer &server);
    ~Handler();

    void on_connect(ftpp::Socket &conn);
    void on_except();
  };

public:
  EventLoop &loop;

private:
  typedef std::list<Handler *> Handlers;
  Handlers _handlers;

  TCPServer();

protected:
  TCPServer(EventLoop &loop, std::string const &host, int port);

public:
  virtual ~TCPServer();

  virtual void on_connect(ftpp::Socket &conn) = 0;
};

} // namespace ftev
