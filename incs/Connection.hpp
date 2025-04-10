/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 23:06:24 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/12 00:32:36 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Cycle.hpp"
#include "configs/Configs.hpp"
#include "structs/Address.hpp"
#include "structs/Request.hpp"

#include <ftev/EventLoop/DeferredDelete.hpp>
#include <ftev/EventLoop/TimerWatcher.hpp>
#include <ftev/Stream/TCPConnection.hpp>
#include <ftpp/urllib/URI.hpp>

#include <map>
#include <string>
#include <vector>

class Connection : public ftev::TCPConnection,
                   public ftev::EventLoop::DeferredDelete {
public:
  enum State { REQUEST, RESPONSE, DONE };

  class Timeout : public ftev::EventLoop::TimerWatcher {
  private:
    Connection &_connection;

    Timeout();

  public:
    Timeout(ftev::EventLoop &loop, Connection &connection);
    ~Timeout();

    void on_timeout();
  };

  static time_t const request_timeout;

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

  void _process();
  bool _process_request();

public:
  Connection(ftev::EventLoop &loop, ftpp::Socket &socket,
             Address const &address, Configs const &configs);
  ~Connection();

  void on_data(std::vector<char> const &data);
  void on_eof();
  void on_drain();
  void on_except();
  void on_release();
};
