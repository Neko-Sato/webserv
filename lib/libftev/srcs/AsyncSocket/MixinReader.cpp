/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MixinReader.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 23:59:53 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/05 04:22:58 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <AsyncSocket/MixinReader.hpp>

namespace ftev {

MixinReader::MixinReader() {
}

MixinReader::~MixinReader() {
}

void MixinReader::on_read() {
  char chank[_chank_size];
  std::size_t size;
  try {
    size = _socket.read(chank, sizeof(chank));
  } catch (...) {
    return;
  }
  if (size == 0) {
    event_t event = get_events() & ~ftpp::BaseSelector::READ;
    if (event)
      modify(event);
    else
      stop();
    on_eof(_buffer);
    return;
  }
  _buffer.insert(_buffer.end(), chank, chank + size);
  on_data(_buffer);
}

} // namespace ftev
