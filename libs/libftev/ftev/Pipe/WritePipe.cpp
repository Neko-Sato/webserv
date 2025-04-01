/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WritePipe.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:45:51 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/02 00:27:18 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Pipe/WritePipe.hpp>

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/macros.hpp>

#include <cassert>
#include <fcntl.h>
#include <stdexcept>

namespace ftev {

WritePipeTransport::Handler::Handler(EventLoop &loop,
                                     WritePipeTransport &transport)
    : IOWatcher(loop), _transport(transport) {
}

WritePipeTransport::Handler::~Handler() {
  if (is_active())
    stop();
}

void WritePipeTransport::Handler::on_read() {
  assert(false);
}

void WritePipeTransport::Handler::on_write() {
  assert(!_transport._buffer.empty());
  try {
    ssize_t size = ::write(_transport._fd, _transport._buffer.data(),
                           _transport._buffer.size());
    if (unlikely(size == -1))
#if defined(FT_SUBJECT_NOT_COMPLIANT)
      throw ftpp::OSError(errno, "write");
#else
      throw std::runtime_error("write: No access to error details");
#endif
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

void WritePipeTransport::Handler::on_except() {
  _transport._protocol.on_except();
}

WritePipeTransport::WritePipeTransport(EventLoop &loop,
                                       WritePipeProtocol &protocol, int fd)
    : _protocol(protocol), _fd(fd), _handler(NULL), _closed(false),
      _draining(false) {
  _handler = new Handler(loop, *this);
}

WritePipeTransport::~WritePipeTransport() {
  delete _handler;
  if (!_closed)
    ::close(_fd);
}

void WritePipeTransport::write(char const *buffer, size_t size) {
  if (_closed)
    throw std::runtime_error("already closed");
  assert(!_draining);
  if (!size)
    return;
#if defined(FT_SUBJECT_NOT_COMPLIANT)
  if (_buffer.empty()) {
    try {
      try {
        ssize_t written = ::write(_fd, buffer, size);
        if (unlikely(written == -1))
          throw ftpp::OSError(errno, "write");
        else if (static_cast<size_t>(written) == size)
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
    _handler->start(_fd, ftpp::Selector::WRITE);
}

void WritePipeTransport::drain() {
  if (_closed)
    throw std::runtime_error("already closed");
  assert(!_draining);
  if (_buffer.empty())
    _protocol.on_drain();
  else
    _draining = true;
}

void WritePipeTransport::close() {
  if (_closed)
    throw std::runtime_error("already closed");
  if (_handler->is_active())
    _handler->stop();
  ::close(_fd);
  _closed = true;
}

} // namespace ftev
