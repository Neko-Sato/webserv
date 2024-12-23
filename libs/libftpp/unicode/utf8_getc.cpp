/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utf8_getc.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 00:11:19 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/24 03:42:42 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unicode/UnicodeError.hpp>
#include <unicode/utf8.hpp>

#include <iomanip>
#include <istream>
#include <sstream>

namespace ftpp {

std::istream &utf8_getc(std::istream &stream, std::string &result) {
  try {
    if (stream) {
      int len = utf8_length(stream.peek());
      len = len < 0 ? 1 : len;
      char utf8[4];
      stream.read(utf8, len);
      std::streamsize size = stream.gcount();
      if (!utf8_isvalid(utf8, size)) {
        std::stringstream ss;
        for (std::size_t i = 0; i < static_cast<std::size_t>(size); ++i) {
          ss << "\\x" << std::nouppercase << std::hex << std::setw(2)
             << std::setfill('0') << (utf8[i] & 0xFF);
        }
        throw UnicodeError("Invalid UTF-8 sequence: " + ss.str());
      }
      result.assign(utf8, size);
    } else
      result.clear();
  } catch (...) {
    stream.setstate(std::ios_base::failbit);
    throw;
  }
  return stream;
}

} // namespace ftpp
