/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MixinReader.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 23:59:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 01:58:59 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <AsyncSocket/MixinReader.hpp>

namespace ftev {

MixinReader::MixinReader() {
}

MixinReader::~MixinReader() {
}

void MixinReader::on_read() {
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
    return;
  }
  on_data(chank);
}

} // namespace ftev
