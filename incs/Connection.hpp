/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 23:06:24 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/02 09:55:38 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "structs/Request.hpp"

#include <AsyncSocket/BaseTCPConnection.hpp>
#include <EventLoop/BaseTimerWatcher.hpp>
#include <urllib/URI.hpp>

#include <map>
#include <string>
#include <vector>

class Connection : public ftev::BaseTCPConnection {
public:
  enum State { REQUEST, BODY, RESPONSE, DONE };

private:
  Server const &_server;

  void _process();

  State _state;
  std::deque<char> _buffer;

  std::size_t _receiveRequestPosition;
  bool _receiveRequest();
  Request _request;
  bool _receiveBody();

public:
  Connection(ftev::EventLoop &loop, ftpp::Socket &socket, Server const &server);
  ~Connection();

  void on_data(std::vector<char> const &data);
  void on_eof();
  void on_drain();
  void on_except();
  void on_release();
};
