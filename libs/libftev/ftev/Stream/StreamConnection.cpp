/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StreamConnection.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:50:15 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 20:43:07 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Stream/StreamConnection.hpp>

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/macros.hpp>

#include <cassert>
#include <fcntl.h>

namespace ftev {

std::size_t const StreamConnection::_chank_size = 4096;

StreamConnection::StreamConnection(EventLoop &loop, ftpp::Socket &socket)
    : IOWatcher(loop), _received_eof(false), _draining(false) {
  _socket.swap(socket);
  int fd = _socket.getSockfd();
  int flags;
  flags = fcntl(fd, F_GETFL);
  if (unlikely(flags == -1))
    throw ftpp::OSError(fd, "fcntl");
  if (unlikely(fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1))
    throw ftpp::OSError(fd, "fcntl");
  start(fd, ftpp::Selector::READ);
}

StreamConnection::~StreamConnection() {
  if (is_active())
    stop();
}

void StreamConnection::on_read() {
  std::vector<char> chank;
  try {
    chank.resize(_chank_size);
    chank.resize(_socket.read(chank.data(), chank.size()));
  } catch (std::exception const &e) {
    ftpp::logger(ftpp::Logger::WARN,
                 ftpp::Format("StreamConnection: {}") % e.what());
    return;
  }
  if (chank.empty()) {
    event_t event = get_events() & ~ftpp::Selector::READ;
    if (event)
      modify(event);
    else
      stop();
    _received_eof = true;
    on_eof();
  } else
    on_data(chank);
}

void StreamConnection::on_write() {
  assert(!_buffer.empty());
  try {
    size_t size = _socket.write(_buffer.data(), _buffer.size());
    _buffer.erase(_buffer.begin(), _buffer.begin() + size);
  } catch (std::exception const &e) {
    ftpp::logger(ftpp::Logger::WARN,
                 ftpp::Format("StreamConnection: {}") % e.what());
    return;
  }
  if (_buffer.empty()) {
    event_t event = get_events() & ~ftpp::Selector::WRITE;
    if (event)
      modify(event);
    else
      stop();
    if (_draining) {
      _draining = false;
      on_drain();
    }
  }
}

void StreamConnection::on_except() {
#if defined(FT_SUBJECT_NOT_COMPLIANT)
  int error = 0;
  socklen_t len = sizeof(error);
  _socket.getsockopt(SOL_SOCKET, SO_ERROR, &error, &len);
  if (error)
    on_error(ftpp::OSError(error));
  else
#endif
    on_error(std::runtime_error("unkown error"));
}

void StreamConnection::resume() {
  assert(!_received_eof);
  if (is_active()) {
    event_t event = get_events();
    if (!(event & ftpp::Selector::READ))
      modify(event | ftpp::Selector::READ);
  } else
    start(_socket.getSockfd(), ftpp::Selector::READ);
}

void StreamConnection::pause() {
  assert(!_received_eof);
  if (is_active()) {
    event_t event = get_events() & ~ftpp::Selector::READ;
    if (event)
      modify(event);
    else
      stop();
  } else
    start(_socket.getSockfd(), ftpp::Selector::READ);
}

void StreamConnection::write(char const *buffer, size_t size) {
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
                   ftpp::Format("StreamConnection: {}") % e.what());
    }
  }
#endif
  _buffer.insert(_buffer.end(), buffer, buffer + size);
  if (is_active()) {
    event_t event = get_events();
    if (!(event & ftpp::Selector::WRITE))
      modify(event | ftpp::Selector::WRITE);
  } else
    start(_socket.getSockfd(), ftpp::Selector::WRITE);
}

void StreamConnection::drain() {
  assert(!_draining);
  if (_buffer.empty())
    on_drain();
  else
    _draining = true;
}

} // namespace ftev
