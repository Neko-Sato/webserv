/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 04:30:35 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/25 04:44:20 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <JsonArray.hpp>
#include <JsonBoolean.hpp>
#include <JsonError.hpp>
#include <JsonNumber.hpp>
#include <JsonObject.hpp>
#include <JsonParser.hpp>
#include <JsonString.hpp>

#include <unicode/surrogate.hpp>
#include <unicode/utf8.hpp>

#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <istream>
#include <sstream>

namespace ftjson {

JsonParser::JsonParser(std::istream &stream) : _lexer(stream) {
}

JsonParser::~JsonParser() {
}

Json JsonParser::_parse() {
  return Json();
}

static inline void _unicode_escape(std::stringstream &ss,
                                   std::string const &str, std::size_t &index) {
  ++index;
  bool valid = true;
  unsigned int code;
  code = std::strtoul(str.substr(index, 4).c_str(), NULL, 16);
  index += 4;
  if (ftpp::ishghtsurrogate(code)) {
    if (str[index] == '\\' && str[index + 1] == 'u') {
      index += 2;
      unsigned int code2;
      code2 = std::strtoul(str.substr(index, 4).c_str(), NULL, 16);
      index += 4;
      if (ftpp::islowsurrogate(code2))
        code = ftpp::surrogatepair(code, code2);
      else
        index -= 6, valid = false;
    } else
      valid = false;
  }
  if (valid)
    ss << ftpp::utf8_codepoint(code);
  else
    ss << "\\u" << std::nouppercase << std::setw(4) << std::setfill('0')
       << std::hex << code;
  --index;
}

std::string JsonParser::_string_dequote(std::string const &str) {
  std::stringstream ss;
  std::size_t index = 0;
  assert(str[index++] == '"');
  for (; index < str.size() - 1; ++index) {
    if (str[index] == '\\') {
      ++index;
      assert(index < str.size() - 1);
      switch (str[index]) {
      case 'b':
        ss.put('\b');
        break;
      case 'f':
        ss.put('\f');
        break;
      case 'n':
        ss.put('\n');
        break;
      case 'r':
        ss.put('\r');
        break;
      case 't':
        ss.put('\t');
        break;
      case 'u':
        _unicode_escape(ss, str, index);
        break;
      default:
        ss.put(str[index]);
        break;
      }
    } else
      ss.put(str[index]);
  }
  assert(str[index] == '"');
  return ss.str();
}

Json JsonParser::parse(char const *str) {
  std::stringstream ss(str);
  return parse(ss);
}

Json JsonParser::parse(std::istream &stream) {
  return JsonParser(stream)._parse();
}

} // namespace ftjson
