/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 23:06:24 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 14:14:42 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/Configs.hpp"
#include "structs/Address.hpp"
#include "structs/Request.hpp"

#include <ftev/Stream/StreamConnection.hpp>
#include <ftpp/urllib/URI.hpp>

#include <map>
#include <string>
#include <vector>

class Connection : public ftev::StreamConnection {
public:
  enum State { REQUEST, RESPONSE, DONE };

private:
  Address _address;
  Configs const &_configs;

  State _state;
  std::deque<char> _buffer;
  bool _bufferClosed;

  void _process();

  std::size_t _receiveRequestPosition;
  bool _receiveRequest();
  Request _request;
  bool _receiveBody();
  bool _completed();

public:
  Connection(ftev::EventLoop &loop, ftpp::Socket &socket,
             Address const &address, Configs const &configs);
  ~Connection();

  void on_data(std::vector<char> const &data);
  void on_eof();
  void on_drain();
  void on_error(std::exception const &exce);
  void on_release();
};
