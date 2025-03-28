/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadPipe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 03:37:58 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/28 15:23:51 by hshimizu         ###   ########.fr       */
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

std::size_t const ReadPipe::_chank_size = 4096;

ReadPipe::ReadPipe(EventLoop &loop, int fd)
    : IOWatcher(loop), _fd(fd), _received_eof(false) {
  start(fd, ftpp::Selector::READ);
}

ReadPipe::~ReadPipe() {
  if (is_active())
    stop();
  close(_fd);
}

void ReadPipe::on_read() {
  std::vector<char> chank;
  try {
    chank.resize(_chank_size);
    ssize_t size = ::read(_fd, chank.data(), chank.size());
    if (size < 0)
      throw ftpp::OSError(errno, "read");
    chank.resize(size);
  } catch (std::exception const &e) {
    ftpp::logger(ftpp::Logger::WARN, ftpp::Format("ReadPipe: {}") % e.what());
    return;
  }
  if (chank.empty()) {
    event_t event = get_events() & ~ftpp::Selector::READ;
    if (event)
      modify(event);
    else
      stop();
    on_eof();
  } else
    on_data(chank);
}

void ReadPipe::on_write() {
  assert(false);
}

void ReadPipe::resume() {
  assert(!_received_eof);
  if (is_active()) {
    event_t event = get_events();
    if (!(event & ftpp::Selector::READ))
      modify(event | ftpp::Selector::READ);
  } else
    start(_fd, ftpp::Selector::READ);
}

void ReadPipe::pause() {
  assert(!_received_eof);
  if (is_active()) {
    event_t event = get_events() & ~ftpp::Selector::READ;
    if (event)
      modify(event);
    else
      stop();
  } else
    start(_fd, ftpp::Selector::READ);
}

} // namespace ftev
