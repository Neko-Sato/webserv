/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cycle.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 17:58:16 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/12 00:11:24 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cycle.hpp"

#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/string/string.hpp>

#include <cassert>

Cycle::Cycle(ftev::StreamConnectionTransport &transport, Configs const &configs,
             Request const &request, Address const &address)
    : _transport(transport), _request(request), _address(address), _task(NULL),
      _reader(NULL), _keepAlive(false) {
  Request::Headers::const_iterator it;
  it = _request.headers.find("connection");
  if (it != _request.headers.end())
    _keepAlive = it->second.back() == "keep-alive";
  it = _request.headers.find("host");
  ServerConf const &serverConf = configs.findServer(
      _address, it != _request.headers.end() ? it->second.back() : "");
  it = _request.headers.find("transfer-encoding");
  if (it != _request.headers.end()) {
    if (it->second.back() == "chunked")
      _reader = new ChankedReader;
    else
      throw std::runtime_error("no support");
  } else {
    it = _request.headers.find("content-length");
    if (it != _request.headers.end()) {
      std::string const &nstr = it->second.back();
      std::size_t len;
      std::size_t n = ftpp::stoul(nstr, &len);
      if (nstr.size() != len)
        throw std::runtime_error("");
      _reader = new ContentLengthReader(n);
    }
  }
  Location const *loc = serverConf.findLocation(_request.method, _request.path);
  try {
    if (!loc)
      throw std::runtime_error("location not found");
    _task = loc->getDetail().createTask(transport);
  } catch (...) {
    delete _reader;
    throw;
  }
  return;
}

Cycle::~Cycle() {
  delete _reader;
  delete _task;
}

void Cycle::bufferUpdate(std::deque<char> &buffer, bool bufferClosed) {
  if (!_reader) {
    _task->on_eof();
    _transport.pause();
    return;
  }
  std::vector<char> tmp;
  _reader->read(buffer, tmp);
  _task->on_data(tmp);
  if (_reader->completed()) {
    _task->on_eof();
    _transport.pause();
    delete _reader;
    _reader = NULL;
  } else if (bufferClosed)
    throw std::runtime_error("");
}

bool Cycle::getKeepAlive() const {
  return _keepAlive;
}
