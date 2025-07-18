/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadPipe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:37:58 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/18 20:05:40 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Pipe/ReadPipe.hpp>

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/macros.hpp>

#include <unistd.h>
#include <cassert>
#include <fcntl.h>

namespace ftev {

ReadPipeProtocol::~ReadPipeProtocol() {
}

ReadPipeTransport::Handler::Handler(EventLoop &loop,
                                    ReadPipeTransport &transport)
    : IOWatcher(loop), _transport(transport) {
  start(transport._fd, ftpp::Selector::READ);
}

ReadPipeTransport::Handler::~Handler() {
  if (getIsActive())
    stop();
}

void ReadPipeTransport::Handler::onRead() {
  std::vector<char> chank;
  try {
    chank.resize(_transport._chankSize);
    ssize_t size = ::read(_transport._fd, chank.data(), chank.size());
    if (unlikely(size == -1))
#if defined(FT_SUBJECT_NOT_COMPLIANT)
      throw ftpp::OSError(errno, "read");
#else
      throw std::runtime_error("read: No access to error details");
#endif
    chank.resize(size);
  } catch (...) {
    return;
  }
  if (!chank.empty())
    _transport._protocol.onData(chank);
}

void ReadPipeTransport::Handler::onWrite() {
  assert(false);
}

void ReadPipeTransport::Handler::onExcept() {
  event_t event = getEvents() & ~ftpp::Selector::READ;
  if (event)
    modify(event);
  else
    stop();
}

std::size_t const ReadPipeTransport::_chankSize = 4096;

ReadPipeTransport::ReadPipeTransport(EventLoop &loop,
                                     ReadPipeProtocol &protocol, int fd)
    : _protocol(protocol), _fd(fd), _handler(NULL), _closed(false) {
  _handler = new Handler(loop, *this);
}

ReadPipeTransport::~ReadPipeTransport() {
  if (!_closed)
    close();
  delete _handler;
}

void ReadPipeTransport::resume() {
  if (_closed)
    throw std::runtime_error("already closed");
  if (_handler->getIsActive()) {
    Handler::event_t event = _handler->getEvents();
    if (!(event & ftpp::Selector::READ))
      _handler->modify(event | ftpp::Selector::READ);
  } else
    _handler->start(_fd, ftpp::Selector::READ);
}

void ReadPipeTransport::pause() {
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

void ReadPipeTransport::close() {
  if (_closed)
    throw std::runtime_error("already closed");
  if (_handler->getIsActive())
    _handler->stop();
  ::close(_fd);
  _closed = true;
}

} // namespace ftev
