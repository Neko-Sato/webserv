/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 23:06:24 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/19 02:09:57 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Reader.hpp"
#include "configs/Configs.hpp"
#include "structs/Address.hpp"
#include "structs/Request.hpp"
#include "tasks/Task.hpp"

#include <ftev/EventLoop/Reaper.hpp>
#include <ftev/EventLoop/TimerWatcher.hpp>
#include <ftev/Stream/TCPConnection.hpp>
#include <ftpp/urllib/URI.hpp>

#include <map>
#include <string>
#include <vector>

class Connection : public ftev::TCPConnection, public ftev::EventLoop::Reaper {
public:
  enum State { REQUEST, RESPONSE, DONE };

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

  class Cycle : private ftpp::NonCopyable {
  private:
    Connection &_connection;
    Task *_task;
    Reader *_reader;

    Cycle();

  public:
    Cycle(Connection &_connection);
    ~Cycle();

    void bufferUpdate();
  };

  static time_t const requestTimeout;

private:
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
