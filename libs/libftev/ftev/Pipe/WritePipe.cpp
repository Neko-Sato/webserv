/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WritePipe.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:45:51 by hshimizu          #+#    #+#             */
/*   Updated: 2025/06/28 03:21:45 by hshimizu         ###   ########.fr       */
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

WritePipeProtocol::~WritePipeProtocol() {
}

WritePipeTransport::Handler::Handler(EventLoop &loop,
                                     WritePipeTransport &transport)
    : IOWatcher(loop), _transport(transport) {
}

WritePipeTransport::Handler::~Handler() {
  if (getIsActive())
    stop();
}

void WritePipeTransport::Handler::onRead() {
  assert(false);
}

void WritePipeTransport::Handler::onWrite() {
  try {
    std::vector<char> chank(
        _transport._buffer.begin(),
        _transport._buffer.begin() +
            std::min(_transport._chankSize, _transport._buffer.size()));
    ssize_t size = ::write(_transport._fd, chank.data(), chank.size());
    if (unlikely(size == -1))
#if defined(FT_SUBJECT_NOT_COMPLIANT)
      throw ftpp::OSError(errno, "write");
#else
      throw std::runtime_error("write: No access to error details");
#endif
    _transport._buffer.erase(_transport._buffer.begin(), _transport._buffer.begin() + size);
  } catch (...) {
  }
  if (_transport._buffer.empty()) {
    event_t event = getEvents() & ~ftpp::Selector::WRITE;
    if (event)
      modify(event);
    else
      stop();
    if (_transport._draining) {
      _transport._draining = false;
      _transport._protocol.onDrain();
    }
  }
}

void WritePipeTransport::Handler::onExcept() {
}

WritePipeTransport::DrainHandler::DrainHandler(EventLoop &loop,
                                               WritePipeTransport &transport)
    : DeferWatcher(loop), _transport(transport) {
}

WritePipeTransport::DrainHandler::~DrainHandler() {
  if (getIsActive())
    cancel();
}

void WritePipeTransport::DrainHandler::onEvent() {
  _transport._draining = false;
  _transport._protocol.onDrain();
}

std::size_t const WritePipeTransport::_chankSize = 4096;

WritePipeTransport::WritePipeTransport(EventLoop &loop,
                                       WritePipeProtocol &protocol, int fd)
    : _protocol(protocol), _fd(fd), _handler(NULL), _drainHandler(NULL),
      _closed(false), _draining(false) {
  try {
    _handler = new Handler(loop, *this);
    _drainHandler = new DrainHandler(loop, *this);
  } catch (...) {
    delete _drainHandler;
    delete _handler;
    throw;
  }
}

WritePipeTransport::~WritePipeTransport() {
  if (!_closed)
    close();
  delete _drainHandler;
  delete _handler;
}

void WritePipeTransport::write(char const *buffer, size_t size) {
  if (_closed)
    throw std::runtime_error("already closed");
  assert(!_draining);
  if (!size)
    return;
  if (_buffer.empty()) {
    try {
      ssize_t written = ::write(_fd, buffer, std::min(size, _chankSize));
      if (unlikely(written == -1))
        throw ftpp::OSError(errno, "write");
      else if (static_cast<size_t>(written) == size)
        return;
      buffer += written;
      size -= written;
    } catch (...) {
    }
  }
  if (_handler->getIsActive()) {
    Handler::event_t event = _handler->getEvents();
    if (!(event & ftpp::Selector::WRITE))
      _handler->modify(event | ftpp::Selector::WRITE);
  } else
    _handler->start(_fd, ftpp::Selector::WRITE);
  _buffer.insert(_buffer.end(), buffer, buffer + size);
}

void WritePipeTransport::drain() {
  assert(!_draining);
  if (_closed)
    throw std::runtime_error("already closed");
  if (_buffer.empty())
    _drainHandler->start();
  _draining = true;
}

void WritePipeTransport::close() {
  if (_closed)
    throw std::runtime_error("already closed");
  if (_handler->getIsActive())
    _handler->stop();
  if (_drainHandler->getIsActive())
    _drainHandler->cancel();
  ::close(_fd);
  _closed = true;
}

} // namespace ftev
