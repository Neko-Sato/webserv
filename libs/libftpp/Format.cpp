/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Format.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:55:26 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/15 23:48:38 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Format.hpp>
#include <ft_iterator.hpp>

namespace ftpp {

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
