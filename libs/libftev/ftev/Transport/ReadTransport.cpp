/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadTransport.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:36:28 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/19 01:35:58 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
  ReadProtocol &protocol = getProtocol();
  if (chank.empty()) {
    stop();
    protocol.on_eof();
  } else {
    protocol.on_data(chank);
  }
}

void ReadTransport::start() {
  EventLoop::BaseIOWatcher &_watcher = _getWatcher();
  if (_watcher.is_active()) {
    EventLoop::BaseIOWatcher::event_t event = _watcher.get_events();
    if (~event & ftpp::BaseSelector::WRITE)
      _watcher.modify(event | ftpp::BaseSelector::READ);
  } else
    _watcher.start(getFd(), ftpp::BaseSelector::READ);
}

void ReadTransport::stop() {
  EventLoop::BaseIOWatcher &_watcher = _getWatcher();
  if (!_watcher.is_active()) {
    EventLoop::BaseIOWatcher::event_t event =
        _watcher.get_events() & ~ftpp::BaseSelector::READ;
    if (event)
      _watcher.modify(event);
    else
      _watcher.stop();
  }
}

} // namespace ftev
