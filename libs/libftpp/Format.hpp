/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Format.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:55:26 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/16 00:27:23 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

// Hmmm, not very beautiful. It's a pain to implement properly.
// Like freeing up the order or something...

namespace ftpp {

void applyFormat(std::ostream &os, std::string const &format);

class Format {
private:
  std::string _fmt;
  std::size_t _pos;
  std::string _res;

  void _next();

public:
  Format(std::string const &fmt = "");
  Format(Format const &rhs);
  ~Format();
  Format &operator=(Format const &rhs);

  void swap(Format &rhs) throw();

  template <typename T> Format &operator%(T const &value);

  operator std::string();
  std::string const &str() const;
  void clear();
};

template <typename T> Format &Format::operator%(T const &value) {
  if (_pos >= _fmt.size())
    throw std::runtime_error("Format: too many arguments");
  std::size_t match = _fmt.find_first_of("{}", _pos);
  if (match == std::string::npos || _fmt[match] == '{')
    throw std::runtime_error("Format: syntax error");
  std::string ctx(_fmt, _pos, match - _pos);
  {
    std::ostringstream oss;
    applyFormat(oss, ctx);
    oss << value;
    if (oss.fail())
      throw std::runtime_error("Format: invalid argument");
    _res += oss.str();
  }
  _pos = match + 1;
  _next();
  return *this;
}

} // namespace ftpp
