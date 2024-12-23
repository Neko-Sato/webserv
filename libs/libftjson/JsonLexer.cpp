/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonLexer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 02:25:31 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/24 23:59:05 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <JsonError.hpp>
#include <JsonLexer.hpp>

#include <ft_algorithm.hpp>
#include <unicode/utf8.hpp>

#include <cctype>
#include <cstring>
#include <sstream>

namespace ftjson {

JsonLexer::JsonLexer(std::istream &stream) : _stream(stream) {
}

JsonLexer::~JsonLexer() {
}

JsonToken JsonLexer::nextToken() {
  try {
    _skip_space();
    char c = _stream.peek();
    switch (c) {
    case -1:
      return JsonToken(JsonToken::END);
    case '{':
      _stream.ignore();
      return JsonToken(JsonToken::LEFT_BRACE);
    case '}':
      _stream.ignore();
      return JsonToken(JsonToken::RIGHT_BRACE);
    case '[':
      _stream.ignore();
      return JsonToken(JsonToken::LEFT_BRACKET);
    case ']':
      _stream.ignore();
      return JsonToken(JsonToken::RIGHT_BRACKET);
    case ',':
      _stream.ignore();
      return JsonToken(JsonToken::COMMA);
    case ':':
      _stream.ignore();
      return JsonToken(JsonToken::COLON);
    case '"':
      return _maybe_string();
    default:
      if (std::isdigit(c) || c == '-')
        return _maybe_number();
      char tmp[5];
      if (!_stream.read(&tmp[0], 4))
        break;
      if (!std::memcmp(tmp, "true", 4))
        return JsonToken(JsonToken::TRUE);
      else if (!std::memcmp(tmp, "null", 4))
        return JsonToken(JsonToken::NULL_);
      if (!_stream.read(&tmp[4], 1))
        break;
      if (!std::memcmp(tmp, "false", 5))
        return JsonToken(JsonToken::FALSE);
    }
    throw JsonError("Unexpected token");
  } catch (...) {
    _stream.setstate(std::ios_base::failbit);
    throw;
  }
}

void JsonLexer::_skip_space() {
  while (std::isspace(_stream.peek()))
    _stream.ignore();
}

JsonToken JsonLexer::_maybe_string() {
  std::stringstream ss;
  bool escape = false;
  ss.put(_stream.get());
  for (std::string tmp; ftpp::utf8_getc(_stream, tmp);) {
    ss << tmp;
    if (tmp.length() == 1) {
      char const &c = tmp[0];
      if (std::iscntrl(c))
        throw JsonError("Bad control character in string literal");
      if (!escape) {
        if (c == '\\') {
          escape = true;
          continue;
        }
        if (c == '"')
          return JsonToken(JsonToken::STRING, ss.str());
      } else if (c == 'u') {
        char tmp[4];
        _stream.read(tmp, 4);
        if (_stream.gcount() != 4 || !ftpp::all_of(tmp, tmp + 4, ::isxdigit))
          throw JsonError("Bad Unicode escape");
        ss.write(tmp, 4);
      }
      escape = false;
    }
  }
  throw JsonError("Unterminated string");
}

JsonToken JsonLexer::_maybe_number() {
  std::stringstream ss;
  if (_stream.peek() == '-') {
    ss.put(_stream.get());
    if (!std::isdigit(_stream.peek()))
      throw JsonError("No number after minus sign");
  }
  if (_stream.peek() == '0')
    throw JsonError("Unexpected number");
  for (char c; (c = _stream.peek()) != -1;) {
    if (!std::isdigit(c))
      break;
    ss.put(_stream.get());
  }
  if (_stream.peek() == '.') {
    ss.put(_stream.get());
    if (!std::isdigit(_stream.peek()))
      throw JsonError("Unterminated fractional number");
    for (char c; (c = _stream.peek()) != -1;) {
      if (!std::isdigit(c))
        break;
      ss.put(_stream.get());
    }
  }
  if (std::tolower(_stream.peek()) == 'e') {
    ss.put(_stream.get());
    if (std::memchr("+-", _stream.peek(), 2)) {
      ss.put(_stream.get());
      if (!std::isdigit(_stream.peek()))
        throw JsonError("Exponent part is missing a number");
    }
    for (char c; (c = _stream.peek()) != -1;) {
      if (!std::isdigit(c))
        break;
      ss.put(_stream.get());
    }
  }
  return JsonToken(JsonToken::NUMBER, ss.str());
}

} // namespace ftjson
