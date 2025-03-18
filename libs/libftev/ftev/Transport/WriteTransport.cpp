/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WriteTransport.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:36:28 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 20:43:48 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Transport/WriteTransport.hpp>

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>

#include <cassert>

namespace ftev {

WriteTransport::WriteTransport(EventLoop &loop) : BaseTransport(loop) {
}

WriteTransport::~WriteTransport() {
}

void WriteTransport::on_write() {
  assert(!_buffer.empty());
  try {
    size_t size = _write(_buffer.data(), _buffer.size());
    _buffer.erase(_buffer.begin(), _buffer.begin() + size);
  } catch (std::exception const &e) {
    ftpp::logger.log(ftpp::Logger::WARN,
                     ftpp::Format("WriteTransport write: {}") % e.what());
    return;
  }
  if (_buffer.empty()) {
    EventLoop::BaseIOWatcher::event_t event =
        _handler.get_events() & ~ftpp::BaseSelector::WRITE;
    if (event)
      _handler.modify(event);
    else
      _handler.stop();
    if (_draining) {
      getProtocol().on_drain();
      _draining = false;
    }
  }
}

void WriteTransport::write(char const *buffer, std::size_t size) {
  assert(!_draining);
  if (!size)
    return;
#if defined(FT_SUBJECT_NOT_COMPLIANT)
  if (_buffer.empty()) {
    try {
      try {
        size_t written = _write(buffer, size);
        if (written == size)
          return;
        buffer += written;
        size -= written;
      } catch (ftpp::OSError const &e) {
        switch (e.get_errno()) {
        case EAGAIN:
          break;
        default:
          throw;
        }
      }
    } catch (std::exception const &e) {
      ftpp::logger.log(ftpp::Logger::WARN,
                       ftpp::Format("WriteTransport write: {}") % e.what());
    }
  }
#endif
  // I was aiming for a strong server.
  // I could not think of a countermeasure for lack of resources here.
  // The server terminates with a THROW in the event loop.
  _buffer.insert(_buffer.end(), buffer, buffer + size);
  // ↑↑↑
  if (_handler.is_active()) {
    EventLoop::BaseIOWatcher::event_t event = _handler.get_events();
    if (!(event & ftpp::BaseSelector::WRITE))
      _handler.modify(event | ftpp::BaseSelector::WRITE);
  } else
    _handler.start(getFd(), ftpp::BaseSelector::WRITE);
}

void WriteTransport::drain() {
  assert(!_draining);
  if (_buffer.empty()) {
    getProtocol().on_drain();
    _draining = false;
  } else {
    _draining = true;
  }
}

} // namespace ftev
