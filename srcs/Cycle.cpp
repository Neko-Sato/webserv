/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cycle.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 17:58:16 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/06 20:09:44 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cycle.hpp"

#include <ftpp/string/string.hpp>

#include <cassert>

Cycle::Cycle(ftev::StreamConnectionTransport &transport, Configs const &configs,
             Request const &request, Address const &address)
    : _transport(transport), _configs(configs), _request(request),
      _address(address), _task(NULL), _reader(NULL) {
  Request::Headers::const_iterator it;
  it = _request.headers.find("host");
  ServerConf const &serverConf = _configs.findServer(
      _address, it != _request.headers.end() ? it->second.back() : "");
  it = _request.headers.find("transfer-encoding");
  if (it != _request.headers.end()) {
    if (it->second.back() == "chank")
      _reader = new ChankReader;
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
  if (!loc)
    throw std::runtime_error("not found");
  _task = loc->createTask();
  if (!_reader)
    _transport.pause();
}

Cycle::~Cycle() {
  delete _reader;
  delete _task;
}

void Cycle::bufferUpdate(std::deque<char> &buffer, bool eof) {
  assert(_reader);
  assert(_task);
  std::vector<char> tmp;
  _reader->read(buffer, tmp);
  if (_reader->completed()) {
    _transport.pause();
    delete _reader;
    _reader = NULL;
  }
  //
  if (eof && !_reader->completed())
    throw std::runtime_error();
}
