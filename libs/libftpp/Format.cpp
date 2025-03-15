/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Format.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:55:26 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/16 00:23:35 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Format.hpp>
#include <ft_iterator.hpp>
#include <ft_string.hpp>

#include <iomanip>

namespace ftpp {

void applyFormat(std::ostream &os, std::string const &format) {
  std::size_t pos = 0;
  while (pos < format.size()) {
    char ch = format[pos];
    switch (ch) {
    case '0':
      os << std::setfill('0');
      ++pos;
      break;
    case '<':
      os << std::left;
      ++pos;
      break;
    case '>':
      os << std::right;
      ++pos;
      break;
    case '^':
      os << std::internal;
      ++pos;
      break;
    case '+':
    case '-':
      os << (ch == '+' ? std::showpos : std::noshowpos);
      ++pos;
      break;
    case '#':
      os << std::showbase;
      ++pos;
      break;
    default:
      if (std::isdigit(ch)) {
        std::size_t nextPos;
        os << std::setw(ftpp::stoul(format.substr(pos), &nextPos));
        pos += nextPos;
      } else if (ch == '.') {
        std::size_t nextPos;
        os << std::setprecision(ftpp::stoul(format.substr(pos + 1), &nextPos));
        pos += nextPos + 1;
      } else {
        throw std::runtime_error("Format: invalid format");
      }
      break;
    }
  }
}

Format::Format(std::string const &fmt) : _fmt(fmt), _pos(0) {
  _next();
}

Format::Format(Format const &rhs)
    : _fmt(rhs._fmt), _pos(rhs._pos), _res(rhs._res) {
}

Format::~Format() {
}

Format &Format::operator=(Format const &rhs) {
  if (this != &rhs)
    Format(rhs).swap(*this);
  return *this;
}

void Format::swap(Format &rhs) throw() {
  _fmt.swap(rhs._fmt);
  _res.swap(rhs._res);
  std::swap(_pos, rhs._pos);
}

Format::operator std::string() {
  return str();
}

std::string const &Format::str() const {
  if (_pos < _fmt.size())
    throw std::runtime_error("Format: not enough arguments");
  return _res;
}

void Format::clear() {
  _res.clear();
  _pos = 0;
  _next();
}

void Format::_next() {
  for (;;) {
    std::size_t match = _fmt.find_first_of("{}", _pos);
    if (match == std::string::npos) {
      _res.append(_fmt, _pos, _fmt.size() - _pos);
      _pos = _fmt.size();
      break;
    }
    _res.append(_fmt, _pos, match - _pos);
    _pos = match + 1;
    if (_pos < _fmt.size() && _fmt[_pos - 1] == _fmt[_pos]) {
      _res.push_back(_fmt[_pos]);
      ++_pos;
    } else
      break;
  }
}

} // namespace ftpp
