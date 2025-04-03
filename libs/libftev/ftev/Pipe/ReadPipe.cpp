/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadPipe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:37:58 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/03 14:59:41 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Pipe/ReadPipe.hpp>

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/macros.hpp>

#include <cassert>
#include <fcntl.h>

namespace ftev {

ReadPipeTransport::Handler::Handler(EventLoop &loop,
                                    ReadPipeTransport &transport)
    : IOWatcher(loop), _transport(transport) {
  start(transport._fd, ftpp::Selector::READ);
}

ReadPipeTransport::Handler::~Handler() {
  if (is_active())
    stop();
}

void ReadPipeTransport::Handler::on_read() {
  std::vector<char> chank;
  try {
    chank.resize(_transport._chank_size);
    ssize_t size = read(_transport._fd, chank.data(), chank.size());
    if (unlikely(size == -1))
      throw ftpp::OSError(errno, "read");
    chank.resize(size);
  } catch (std::exception const &e) {
    ftpp::logger(ftpp::Logger::WARN,
                 ftpp::Format("ReadPipeTransport: {}") % e.what());
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

void ReadPipeTransport::Handler::on_write() {
  assert(false);
}

void ReadPipeTransport::Handler::on_except() {
  _transport._protocol.on_except();
}

std::size_t const ReadPipeTransport::_chank_size = 4096;

ReadPipeTransport::ReadPipeTransport(EventLoop &loop,
                                     ReadPipeProtocol &protocol, int fd)
    : _protocol(protocol), _fd(fd), _handler(NULL), _closed(false) {
  _handler = new Handler(loop, *this);
}

ReadPipeTransport::~ReadPipeTransport() {
  delete _handler;
  if (!_closed)
    ::close(_fd);
}

void ReadPipeTransport::resume() {
  if (_closed)
    throw std::runtime_error("already closed");
  if (_handler->is_active()) {
    Handler::event_t event = _handler->get_events();
    if (!(event & ftpp::Selector::READ))
      _handler->modify(event | ftpp::Selector::READ);
  } else
    _handler->start(_fd, ftpp::Selector::READ);
}

void ReadPipeTransport::pause() {
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

void ReadPipeTransport::close() {
  if (_closed)
    throw std::runtime_error("already closed");
  if (_handler->is_active())
    _handler->stop();
  ::close(_fd);
  _closed = true;
}

} // namespace ftev
