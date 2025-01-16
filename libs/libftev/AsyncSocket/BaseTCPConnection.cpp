/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTCPConnection.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 23:59:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/17 02:18:51 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <AsyncSocket/BaseTCPConnection.hpp>

#include <cassert>

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
  std::vector<char> chank(_chank_size);
  try {
    chank.resize(_socket.read(chank.data(), chank.size()));
  } catch (...) {
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
  size_t size = _socket.write(_buffer.data(), _buffer.size());
  _buffer.erase(_buffer.begin(), _buffer.begin() + size);
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
