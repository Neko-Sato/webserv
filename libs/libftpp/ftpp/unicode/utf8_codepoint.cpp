/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utf8_codepoint.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 16:38:00 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:55:34 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/unicode/UnicodeError.hpp>
#include <ftpp/unicode/utf8.hpp>

#include <string>

namespace ftpp {

std::string utf8_codepoint(unsigned int codepoint) {
  char utf8[4];
  std::size_t len = 0;
  if (codepoint <= 0x7F) {
    utf8[0] = static_cast<char>(codepoint);
    len = 1;
  } else if (codepoint <= 0x7FF) {
    utf8[0] = static_cast<char>((codepoint >> 6) | 0xC0);
    utf8[1] = static_cast<char>((codepoint & 0x3F) | 0x80);
    len = 2;
  } else if (codepoint <= 0xFFFF) {
    utf8[0] = static_cast<char>((codepoint >> 12) | 0xE0);
    utf8[1] = static_cast<char>(((codepoint >> 6) & 0x3F) | 0x80);
    utf8[2] = static_cast<char>((codepoint & 0x3F) | 0x80);
    len = 3;
  } else if (codepoint <= 0x10FFFF) {
    utf8[0] = static_cast<char>((codepoint >> 18) | 0xF0);
    utf8[1] = static_cast<char>(((codepoint >> 12) & 0x3F) | 0x80);
    utf8[2] = static_cast<char>(((codepoint >> 6) & 0x3F) | 0x80);
    utf8[3] = static_cast<char>((codepoint & 0x3F) | 0x80);
    len = 4;
  } else
    throw UnicodeError("invalid codepoint");
  return std::string(utf8, len);
}

} // namespace ftpp
