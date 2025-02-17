/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTCPConnection.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 23:59:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/18 01:33:27 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <AsyncSocket/BaseTCPConnection.hpp>

#include <cassert>
#include <iostream>

namespace ftev {

std::size_t const BaseTCPConnection::_chank_size = 4096;

BaseTCPConnection::BaseTCPConnection(EventLoop &loop, int domain, int type,
                                     int protocol)
    : BaseAsyncSocket(loop, domain, type, protocol), _draining(false) {
  start(_socket.getSockfd(), ftpp::BaseSelector::READ);
}

BaseTCPConnection::BaseTCPConnection(EventLoop &loop, ftpp::Socket &socket)
    : BaseAsyncSocket(loop, socket), _draining(false) {
  start(_socket.getSockfd(), ftpp::BaseSelector::READ);
}

BaseTCPConnection::~BaseTCPConnection() {
}

void BaseTCPConnection::on_read() {
  std::vector<char> chank;
  try {
    chank.resize(_chank_size);
    chank.resize(_socket.read(chank.data(), chank.size()));
  } catch (std::exception const &e) {
    std::cerr << "TCPConnection read: " << e.what() << std::endl;
    return;
  }
  if (chank.empty()) {
    event_t event = get_events() & ~ftpp::BaseSelector::READ;
    if (event)
      modify(event);
    else
      stop();
    on_eof();
  } else {
    on_data(chank);
  }
}

void BaseTCPConnection::on_write() {
  try {
    size_t size = _socket.write(_buffer.data(), _buffer.size());
    _buffer.erase(_buffer.begin(), _buffer.begin() + size);
  } catch (std::exception const &e) {
    std::cerr << "TCPConnection write: " << e.what() << std::endl;
    return;
  }
  if (_buffer.empty()) {
    event_t event = get_events() & ~ftpp::BaseSelector::WRITE;
    if (event)
      modify(event);
    else
      stop();
    if (_draining) {
      on_drain();
      _draining = false;
    }
  }
}

void BaseTCPConnection::write(char const *buffer, size_t size) {
  assert(!_draining);
  if (!size)
    return;
  _buffer.insert(_buffer.end(), buffer, buffer + size);
  if (is_active()) {
    event_t event = get_events();
    if (!(event & ftpp::BaseSelector::WRITE))
      modify(event | ftpp::BaseSelector::WRITE);
  } else
    start(_socket.getSockfd(), ftpp::BaseSelector::WRITE);
}

void BaseTCPConnection::drain() {
  assert(!_draining);
  if (_buffer.empty())
    on_drain();
  else
    _draining = true;
}

} // namespace ftev
