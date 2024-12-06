/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MixinWriter.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 23:59:53 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/06 10:22:35 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <AsyncSocket/MixinWriter.hpp>

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
    if (_draining)
      on_drain();
  }
}

void MixinWriter::write(char const *buffer, size_t size) {
  if (!size)
    return;
  _draining = false;
  _buffer.insert(_buffer.end(), buffer, buffer + size);
  if (is_active()) {
    event_t event = get_events();
    if (!(event & ftpp::BaseSelector::WRITE))
      modify(event | ftpp::BaseSelector::WRITE);
  } else
    start(_socket.get_sockfd(), ftpp::BaseSelector::WRITE);
}

void MixinWriter::drain() {
  _draining = true;
  if (_buffer.empty())
    on_drain();
}

} // namespace ftev
