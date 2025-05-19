/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cycle.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 03:13:29 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/19 21:48:48 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "App.hpp"
#include "Connection.hpp"

class Connection::Cycle : private ftpp::NonCopyable {
private:
  enum State { RESPONSE, BODY, DONE };

  Connection &_connection;
  State _state;
  ServerConf const *_serverConf;
  App *_app;
  Reader *_reader;
  Writer *_writer;
  bool _eof;

  Cycle();

public:
  Cycle(Connection &connection);
  ~Cycle();

  void bufferUpdate();

  ftev::EventLoop &getLoop();
  ServerConf const &getServerConf() const;
  Request const &getRequest() const;

  void send(int code, Headers const &headers);
  void send(char const *data, std::size_t size, bool more);
  void sendErrorPage(int code);
  void abort();
};
