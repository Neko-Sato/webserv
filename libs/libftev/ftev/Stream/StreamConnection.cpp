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

StreamConnectionProtocol::~StreamConnectionProtocol() {
}

StreamConnectionTransport::Handler::Handler(
    EventLoop &loop, StreamConnectionTransport &transport)
    : IOWatcher(loop), _transport(transport) {
  start(transport._socket.getSockfd(), ftpp::Selector::READ);
}

StreamConnectionTransport::Handler::~Handler() {
  if (getIsActive())
    stop();
}

void StreamConnectionTransport::Handler::onRead() {
  std::vector<char> chank;
  try {
    chank.resize(_transport._chankSize);
    chank.resize(_transport._socket.read(chank.data(), chank.size()));
  } catch (...) {
    return;
  }
  if (chank.empty()) {
    event_t event = getEvents() & ~ftpp::Selector::READ;
    if (event)
      modify(event);
    else
      stop();
    _transport._protocol.onEof();
  } else
    _transport._protocol.onData(chank);
}

void StreamConnectionTransport::Handler::onWrite() {
  try {
    size_t size = _transport._socket.write(_transport._buffer.data(),
                                           _transport._buffer.size());
    _transport._buffer.erase(_transport._buffer.begin(),
                             _transport._buffer.begin() + size);
  } catch (...) {
    return;
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

void StreamConnectionTransport::Handler::onExcept() {
#if defined(FT_SUBJECT_NOT_COMPLIANT)
  int err;
  socklen_t errlen = sizeof(err);
  _transport._socket.getsockopt(SOL_SOCKET, SO_ERROR, &err, &errlen);
  if (err)
    _transport._protocol.onError(ftpp::OSError(err));
  else
#endif
    _transport._protocol.onError(std::runtime_error("Unkown error"));
}

StreamConnectionTransport::DrainHandler::DrainHandler(
    EventLoop &loop, StreamConnectionTransport &transport)
    : DeferWatcher(loop), _transport(transport) {
}

StreamConnectionTransport::DrainHandler::~DrainHandler() {
  if (getIsActive())
    cancel();
}

void StreamConnectionTransport::DrainHandler::onEvent() {
  _transport._draining = false;
  _transport._protocol.onDrain();
}

std::size_t const StreamConnectionTransport::_chankSize = 4096;

StreamConnectionTransport::StreamConnectionTransport(
    EventLoop &loop, StreamConnectionProtocol &protocol, ftpp::Socket &socket)
    : _protocol(protocol), _handler(NULL), _drainHandler(NULL),
      _draining(false), _closed(false) {
  _socket.swap(socket);
  try {
    _handler = new Handler(loop, *this);
    _drainHandler = new DrainHandler(loop, *this);
  } catch (...) {
    delete _drainHandler;
    delete _handler;
    throw;
  }
}

StreamConnectionTransport::~StreamConnectionTransport() {
  delete _drainHandler;
  delete _handler;
}

void StreamConnectionTransport::resume() {
  if (_closed)
    throw std::runtime_error("already closed");
  if (_handler->getIsActive()) {
    Handler::event_t event = _handler->getEvents();
    if (!(event & ftpp::Selector::READ))
      _handler->modify(event | ftpp::Selector::READ);
  } else
    _handler->start(_socket.getSockfd(), ftpp::Selector::READ);
}

void StreamConnectionTransport::pause() {
  if (_closed)
    throw std::runtime_error("already closed");
  if (_handler->getIsActive()) {
    Handler::event_t event = _handler->getEvents() & ~ftpp::Selector::READ;
    if (event)
      _handler->modify(event);
    else
      _handler->stop();
  }
}

void StreamConnectionTransport::write(char const *buffer, size_t size) {
  assert(!_draining);
  if (_closed)
    throw std::runtime_error("already closed");
  if (!size)
    return;
  if (_buffer.empty()) {
    try {
      size_t written = _socket.write(buffer, size);
      if (written == size)
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
    _handler->start(_socket.getSockfd(), ftpp::Selector::WRITE);
  _buffer.insert(_buffer.end(), buffer, buffer + size);
}

void StreamConnectionTransport::drain() {
  assert(!_draining);
  if (_closed)
    throw std::runtime_error("already closed");
  if (_buffer.empty())
    _drainHandler->start();
  _draining = true;
}

void StreamConnectionTransport::close() {
  if (_closed)
    throw std::runtime_error("already closed");
  if (_handler->getIsActive())
    _handler->stop();
  if (_drainHandler->getIsActive())
    _drainHandler->cancel();
  _socket.close();
  _closed = true;
}

} // namespace ftev
