/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StreamTransport.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:50:15 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/31 18:03:36 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Stream/StreamConnection.hpp>
#include <ftev/utils/utils.hpp>

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/macros.hpp>

#include <cassert>
#include <fcntl.h>

namespace ftev {

StreamConnectionTransport::Handler::Handler(
    EventLoop &loop, StreamConnectionTransport &transport)
    : IOWatcher(loop), _transport(transport) {
  start(transport._socket.getSockfd(), ftpp::Selector::READ);
}

StreamConnectionTransport::Handler::~Handler() {
  if (is_active())
    stop();
}

void StreamConnectionTransport::Handler::on_read() {
  std::vector<char> chank;
  try {
    chank.resize(_transport._chank_size);
    chank.resize(_transport._socket.read(chank.data(), chank.size()));
  } catch (std::exception const &e) {
    ftpp::logger(ftpp::Logger::WARN,
                 ftpp::Format("StreamTransport: {}") % e.what());
    return;
  }
  if (chank.empty()) {
    event_t event = get_events() & ~ftpp::Selector::READ;
    if (event)
      modify(event);
    else
      stop();
    _transport._protocol.on_eof();
  } else
    _transport._protocol.on_data(chank);
}

void StreamConnectionTransport::Handler::on_write() {
  assert(!_transport._buffer.empty());
  try {
    size_t size = _transport._socket.write(_transport._buffer.data(),
                                           _transport._buffer.size());
    _transport._buffer.erase(_transport._buffer.begin(),
                             _transport._buffer.begin() + size);
  } catch (std::exception const &e) {
    ftpp::logger(ftpp::Logger::WARN,
                 ftpp::Format("StreamTransport: {}") % e.what());
    return;
  }
  if (_transport._buffer.empty()) {
    event_t event = get_events() & ~ftpp::Selector::WRITE;
    if (event)
      modify(event);
    else
      stop();
    if (_transport._draining) {
      _transport._draining = false;
      _transport._protocol.on_drain();
    }
  }
}

void StreamConnectionTransport::Handler::on_except() {
  _transport._protocol.on_except();
}

std::size_t const StreamConnectionTransport::_chank_size = 4096;

StreamConnectionTransport::StreamConnectionTransport(
    EventLoop &loop, StreamConnectionProtocol &protocol, ftpp::Socket &socket)
    : _protocol(protocol), _handler(NULL), _closed(false), _draining(false) {
  _socket.swap(socket);
  _handler = new Handler(loop, *this);
}

StreamConnectionTransport::~StreamConnectionTransport() {
  delete _handler;
}

void StreamConnectionTransport::resume() {
  if (_closed)
    throw std::runtime_error("already closed");
  if (_handler->is_active()) {
    Handler::event_t event = _handler->get_events();
    if (!(event & ftpp::Selector::READ))
      _handler->modify(event | ftpp::Selector::READ);
  } else
    _handler->start(_socket.getSockfd(), ftpp::Selector::READ);
}

void StreamConnectionTransport::pause() {
  if (_closed)
    throw std::runtime_error("already closed");
  if (_handler->is_active()) {
    Handler::event_t event = _handler->get_events() & ~ftpp::Selector::READ;
    if (event)
      _handler->modify(event);
    else
      _handler->stop();
  }
}

void StreamConnectionTransport::write(char const *buffer, size_t size) {
  if (_closed)
    throw std::runtime_error("already closed");
  assert(!_draining);
  if (!size)
    return;
#if defined(FT_SUBJECT_NOT_COMPLIANT)
  if (_buffer.empty()) {
    try {
      try {
        size_t written = _socket.write(buffer, size);
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
      ftpp::logger(ftpp::Logger::WARN,
                   ftpp::Format("StreamTransport: {}") % e.what());
    }
  }
#endif
  _buffer.insert(_buffer.end(), buffer, buffer + size);
  if (_handler->is_active()) {
    Handler::event_t event = _handler->get_events();
    if (!(event & ftpp::Selector::WRITE))
      _handler->modify(event | ftpp::Selector::WRITE);
  } else
    _handler->start(_socket.getSockfd(), ftpp::Selector::WRITE);
}

void StreamConnectionTransport::drain() {
  if (_closed)
    throw std::runtime_error("already closed");
  assert(!_draining);
  if (_buffer.empty())
    _protocol.on_drain();
  else
    _draining = true;
}

void StreamConnectionTransport::close() {
  if (_closed)
    throw std::runtime_error("already closed");
  if (_handler->is_active())
    _handler->stop();
  _socket.close();
  _closed = true;
}

} // namespace ftev
