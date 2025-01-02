/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MixinWriter.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 23:59:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/02 17:32:43 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <AsyncSocket/MixinWriter.hpp>

#include <cassert>

namespace ftev {

MixinWriter::MixinWriter() : _draining(false) {
}

MixinWriter::~MixinWriter() {
}

void MixinWriter::on_write() {
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

void MixinWriter::write(char const *buffer, size_t size) {
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

void MixinWriter::drain() {
  assert(!_draining);
  if (_buffer.empty())
    on_drain();
  else
    _draining = true;
}

} // namespace ftev
