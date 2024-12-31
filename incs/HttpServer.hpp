/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 00:49:08 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 01:56:26 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/Configs.hpp"

#include <AsyncSocket/BaseAsyncStreamConnection.hpp>
#include <AsyncSocket/BaseAsyncStreamServer.hpp>
#include <AsyncSocket/BaseTCPServer.hpp>
#include <EventLoop.hpp>

class HttpServer : public ftev::BaseTCPServer {
private:
  class Server : public ftev::BaseAsyncStreamServer {
  private:
    Configs &_configs;

  public:
    Server(ftev::EventLoop &loop, int domain, int type, int protocol,
           Configs &configs);
    ~Server();

    void on_accept(int sockfd, sockaddr const *addr);
    void on_except();
  };

  class Connection : public ftev::BaseAsyncStreamConnection {
  private:
    enum State {
      REQUEST,
      HEADERS,
      BODY,
      RESPONSE,
      DONE,
    };

    struct Request {
      std::string method;
      std::string path;
      std::string version;
      std::map<std::string, std::string> headers;
    };

    Configs &_configs;
    State _state;
    Request _request;

  public:
    Connection(ftev::EventLoop &loop, int connfd, Configs &configs);
    ~Connection();

    void on_data(std::vector<char> const &data);
    void on_eof();
    void on_drain();
    void on_except();
    void on_release();
  };

  Configs &_configs;

public:
  HttpServer(ftev::EventLoop &loop, char const *host, int port,
             Configs &configs);
  ~HttpServer();

  Server *create_server(int domain, int type, int protocol);
};
