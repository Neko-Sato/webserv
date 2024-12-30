/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EchoServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 04:31:15 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/30 17:18:56 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <AsyncSocket/BaseAsyncStreamConnection.hpp>
#include <AsyncSocket/BaseAsyncStreamServer.hpp>
#include <AsyncSocket/BaseTCPServer.hpp>
#include <EventLoop.hpp>

namespace ftev {

class EchoServer : public ftev::BaseTCPServer {
private:
  class Server : public ftev::BaseAsyncStreamServer {
  public:
    Server(ftev::EventLoop &loop, int domain, int type, int protocol);
    ~Server();

    void on_accept(int sockfd, sockaddr const *addr);
    void on_except();
  };

  class Connection : public ftev::BaseAsyncStreamConnection {
  public:
    Connection(ftev::EventLoop &loop, int connfd);
    ~Connection();

    void on_data(std::deque<char> &data);
    void on_eof(std::deque<char> &data);
    void on_drain();
    void on_except();
    void on_release();
  };

public:
  EchoServer(ftev::EventLoop &loop, char const *host, int port);
  ~EchoServer();

  Server *create_server(int domain, int type, int protocol);
};

} // namespace ftev
