/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 21:52:35 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/26 02:03:44 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/string/string.hpp>

#include <cmath>
#include <limits>
#include <stdexcept>

std::size_t parseSize(std::string const &str) {
  if (str.empty() || !isdigit(str[0]))
    throw std::invalid_argument("invalid size");
  double size;
  std::size_t unit;
  {
    std::size_t pos;
    size = ftpp::stod(str, &pos);
    std::string tmp(str, pos, std::string::npos);
    if (tmp.empty())
      unit = 1;
    else if (tmp == "k" || tmp == "K")
      unit = 1ul << 10;
    else if (tmp == "m" || tmp == "M")
      unit = 1ul << 20;
    else if (tmp == "g" || tmp == "G")
      unit = 1ul << 30;
    else
      throw std::runtime_error("invalid size unit");
  }
  if (size > std::numeric_limits<std::size_t>::max() / unit)
    throw std::overflow_error("size overflow");
  return static_cast<std::size_t>(size * unit);
}

char const *resetColorEscape = "\x1b[0m";

char const *statusColorEscape(int code) {
  switch (code / 100) {
  case 1:
    return "\x1b[97m";
  case 2:
    return "\x1b[32m";
  case 3:
    return "\x1b[33m";
  case 4:
    return "\x1b[31m";
  case 5:
    return "\x1b[91m";
  default:
    return "\x1b[0m";
  }
}
