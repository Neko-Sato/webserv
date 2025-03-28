/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WritePipe.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:45:51 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/28 15:24:00 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Pipe/WritePipe.hpp>

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/macros.hpp>

#include <cassert>
#include <fcntl.h>

namespace ftev {

WritePipe::WritePipe(EventLoop &loop, int fd)
    : IOWatcher(loop), _fd(fd), _draining(false) {
}

WritePipe::~WritePipe() {
  if (is_active())
    stop();
}

void WritePipe::on_read() {
  assert(false);
}

void WritePipe::on_write() {
  assert(!_buffer.empty());
  try {
    ssize_t size = ::write(_fd, _buffer.data(), _buffer.size());
    if (size < 0)
      throw ftpp::OSError(errno, "write");
    _buffer.erase(_buffer.begin(), _buffer.begin() + size);
  } catch (std::exception const &e) {
    ftpp::logger(ftpp::Logger::WARN, ftpp::Format("WritePipe: {}") % e.what());
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

void WritePipe::write(char const *buffer, size_t size) {
  assert(!_draining);
  if (!size)
    return;
#if defined(FT_SUBJECT_NOT_COMPLIANT)
  if (_buffer.empty()) {
    try {
      try {
        ssize_t written = ::write(_fd, buffer, size);
        if (written < 0)
          throw ftpp::OSError(errno, "write");
        if (static_cast<std::size_t>(written) == size)
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
                   ftpp::Format("Write Pipe: {}") % e.what());
    }
  }
#endif
  _buffer.insert(_buffer.end(), buffer, buffer + size);
  if (is_active()) {
    event_t event = get_events();
    if (!(event & ftpp::Selector::WRITE))
      modify(event | ftpp::Selector::WRITE);
  } else
    start(_fd, ftpp::Selector::WRITE);
}

void WritePipe::drain() {
  assert(!_draining);
  if (_buffer.empty())
    on_drain();
  else
    _draining = true;
}

} // namespace ftev
