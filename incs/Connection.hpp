/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 23:06:24 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/30 13:26:12 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Reader.hpp"
#include "Writer.hpp"
#include "configs/Configs.hpp"
#include "structs/Address.hpp"
#include "structs/Request.hpp"
#include "structs/Response.hpp"

#include <ftev/EventLoop/Reaper.hpp>
#include <ftev/EventLoop/TimerWatcher.hpp>
#include <ftev/Stream/TCPConnection.hpp>
#include <ftpp/urllib/URI.hpp>

#include <map>
#include <string>
#include <vector>

class Connection : public ftev::TCPConnection, public ftev::EventLoop::Reaper {
public:
  class Timeout : public ftev::EventLoop::TimerWatcher {
  private:
    Connection &_connection;

    Timeout();

  public:
    Timeout(ftev::EventLoop &loop, Connection &connection);
    ~Timeout();

    void onTimeout();
  };

  class Complete : public ftev::EventLoop::DeferWatcher {
  private:
    Connection &_connection;

  public:
    Complete(ftev::EventLoop &loop, Connection &connection);
    ~Complete();

    void onEvent();
  };

  class Cycle;

  static time_t const requestTimeout;
  using TCPConnection::loop;

private:
  enum State { REQUEST, RESPONSE, DONE };

  Address _address;
  Configs const &_configs;

  State _state;
  std::deque<char> _buffer;
  bool _bufferClosed;

  std::size_t _receiveRequestPosition;
  Request _request;
  Cycle *_cycle;
  Timeout *_timeout;
  Complete *_complete;
  bool _keepAlive;

  void _process();
  bool _processRequest();

public:
  Connection(ftev::EventLoop &loop, ftpp::Socket &socket,
             Address const &address, Configs const &configs);
  ~Connection();

  void onData(std::vector<char> const &data);
  void onEof();
  void onDrain();
  void onExcept();
  void onRelease();
};
