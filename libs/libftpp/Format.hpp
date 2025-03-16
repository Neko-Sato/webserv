/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Format.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:55:26 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/16 23:24:13 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

// Hmmm, not very beautiful. It's a pain to implement properly.
// I defined it just for the sake of readability, but if I were to do the same
// thing properly in c++98, it would be as follows Which is Beautiful, isn't it?
// ```
// ftpp::Format("n1: {}, n2: {}") % n1 % n2;
// (std::ostringstream() << n1: " << n << ", n2: " << n2).str()
// ```
// It could be a legacy...

namespace ftpp {

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
  _res += (std::ostringstream() << value).str();
  _pos = match + 1;
  _next();
  return *this;
}

std::ostream &operator<<(std::ostream &os, Format const &fmt);

} // namespace ftpp
