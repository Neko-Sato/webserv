/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:20:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/17 02:02:26 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Reader.hpp"
#include "constants.hpp"

#include <ftpp/algorithm.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/string/string.hpp>

#include <algorithm>

Reader::Reader() {
}

Reader::~Reader() {
}

ContentLengthReader::ContentLengthReader(std::size_t size) : _size(size) {
}

ContentLengthReader::~ContentLengthReader() {
}

void ContentLengthReader::read(std::deque<char> &buffer,
                               std::vector<char> &res) {
  std::size_t size = std::min(buffer.size(), _size);
  res.insert(res.end(), buffer.begin(), buffer.begin() + size);
  _size -= size;
  buffer.erase(buffer.begin(), buffer.begin() + size);
}

bool ContentLengthReader::completed() const {
  return !_size;
}

ChankedReader::ChankedReader() : _state(READ_SIZE), _pos(0), _size(0), _eof(false) {
}

ChankedReader::~ChankedReader() {
}

void ChankedReader::read(std::deque<char> &buffer, std::vector<char> &res) {
  for (;;) {
    switch (_state) {
    case READ_SIZE: {
      if (buffer.size() - _pos < CRLF.size())
        return;
      std::deque<char>::iterator match = std::search(
          buffer.begin() + _pos, buffer.end(), CRLF.begin(), CRLF.end());
      if (match == buffer.end()) {
        _pos = buffer.size() - CRLF.size();
        return;
      }
      _pos = 0;
      std::string tmp(buffer.begin(), match);
      buffer.erase(buffer.begin(), match + CRLF.size());
      std::size_t idx;
      _size = ftpp::stoul(tmp, &idx, 16);
      if (tmp.size() != idx)
        throw std::runtime_error("invalid chunk size");
      if (_size == 0) {
        _eof = true;
        _state = READ_CRLF;
      } else
        _state = READ_DATA;
    } break;
    case READ_DATA: {
      std::size_t size = std::min(buffer.size(), _size);
      res.insert(res.end(), buffer.begin(), buffer.begin() + size);
      _size -= size;
      buffer.erase(buffer.begin(), buffer.begin() + size);
      if (_size)
        return;
      _state = READ_CRLF;
    } break;
    case READ_CRLF:
      if (buffer.size() < CRLF.size())
        return;
      if (!ftpp::starts_with(buffer, CRLF))
        throw std::runtime_error("no CRLF");
      buffer.erase(buffer.begin(), buffer.begin() + CRLF.size());
      if (_eof)
        _state = END;
      else
        _state = READ_SIZE;
      break;
    case END:
      return;
    }
  }
}

bool ChankedReader::completed() const {
  return _state == END;
}
