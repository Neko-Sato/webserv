/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 02:03:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 06:25:51 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/Stream/StreamServer.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>

#include <list>
#include <string>

namespace ftev {

class TCPServer : private ftpp::NonCopyable {
public:
  class Handler : public StreamServer {
  private:
    TCPServer &_server;

  public:
    Handler(EventLoop &loop, ftpp::Socket &socket, TCPServer &server);
    ~Handler();

    void on_connect(ftpp::Socket &conn);
    void on_error(std::exception const &exce);
    void on_release();
  };

  EventLoop &loop;

private:
  typedef std::list<Handler *> Handlers;
  Handlers _handlers;

  TCPServer();

public:
  TCPServer(EventLoop &loop, std::string const &host, int port);
  virtual ~TCPServer();

  virtual void on_connect(ftpp::Socket &conn) = 0;
};

} // namespace ftev
