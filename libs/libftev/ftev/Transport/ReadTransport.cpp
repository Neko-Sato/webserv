/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadTransport.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:36:28 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 20:43:54 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Protocol/ReadProtocol.hpp>
#include <ftev/Transport/ReadTransport.hpp>

#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>

#include <vector>

namespace ftev {

std::size_t const ReadTransport::_chank_size = 4096;

ReadTransport::ReadTransport(EventLoop &loop) : BaseTransport(loop) {
}

ReadTransport::~ReadTransport() {
}

void ReadTransport::on_read() {
  std::vector<char> chank;
  try {
    chank.resize(_chank_size);
    chank.resize(_read(chank.data(), chank.size()));
  } catch (std::exception const &e) {
    ftpp::logger.log(ftpp::Logger::WARN,
                     ftpp::Format("ReadTransport read: {}") % e.what());
    return;
  }
  if (chank.empty()) {
    pause();
    getProtocol().on_eof();
  } else {
    getProtocol().on_data(chank);
  }
}

void ReadTransport::resume() {
  if (_handler.is_active()) {
    EventLoop::BaseIOWatcher::event_t event = _handler.get_events();
    if (~event & ftpp::BaseSelector::WRITE)
      _handler.modify(event | ftpp::BaseSelector::READ);
  } else
    _handler.start(getFd(), ftpp::BaseSelector::READ);
}

void ReadTransport::pause() {
  if (!_handler.is_active()) {
    EventLoop::BaseIOWatcher::event_t event =
        _handler.get_events() & ~ftpp::BaseSelector::READ;
    if (event)
      _handler.modify(event);
    else
      _handler.stop();
  }
}

} // namespace ftev
