/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonLexer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 02:25:31 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:55:51 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftjson/JsonError.hpp>
#include <ftjson/JsonLexer.hpp>

#include <ftpp/algorithm.hpp>
#include <ftpp/unicode/utf8.hpp>

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
    _stream >> std::ws;
    int c = _stream.peek();
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
      return _maybe_keyword();
    }
  } catch (...) {
    _stream.setstate(std::ios_base::failbit);
    throw;
  }
}

JsonToken JsonLexer::_maybe_string() {
  std::ostringstream oss;
  bool escape = false;
  oss.put(_stream.get());
  for (int c; (c = _stream.peek()) != -1;) {
    oss.put(_stream.get());
    if (std::iscntrl(c))
      throw JsonError("Bad control character in string literal");
    if (!escape) {
      if (c == '\\') {
        escape = true;
        continue;
      }
      if (c == '"')
        return JsonToken(JsonToken::STRING, oss.str());
    } else if (c == 'u') {
      char tmp[4];
      _stream.read(tmp, 4);
      if (_stream.gcount() != 4 || !ftpp::all_of(tmp, tmp + 4, ::isxdigit))
        throw JsonError("Bad Unicode escape");
      oss.write(tmp, 4);
    }
    escape = false;
  }
  throw JsonError("Unterminated string");
}

JsonToken JsonLexer::_maybe_number() {
  std::ostringstream oss;
  if (_stream.peek() == '-') {
    oss.put(_stream.get());
    if (!std::isdigit(_stream.peek()))
      throw JsonError("No number after minus sign");
  }
  if (_stream.peek() == '0')
    throw JsonError("Unexpected number");
  for (int c; (c = _stream.peek()) != -1;) {
    if (!std::isdigit(c))
      break;
    oss.put(_stream.get());
  }
  if (_stream.peek() == '.') {
    oss.put(_stream.get());
    if (!std::isdigit(_stream.peek()))
      throw JsonError("Unterminated fractional number");
    for (int c; (c = _stream.peek()) != -1;) {
      if (!std::isdigit(c))
        break;
      oss.put(_stream.get());
    }
  }
  if (std::tolower(_stream.peek()) == 'e') {
    oss.put(_stream.get());
    if (std::memchr("+-", _stream.peek(), 2)) {
      oss.put(_stream.get());
      if (!std::isdigit(_stream.peek()))
        throw JsonError("Exponent part is missing a number");
    }
    for (int c; (c = _stream.peek()) != -1;) {
      if (!std::isdigit(c))
        break;
      oss.put(_stream.get());
    }
  }
  return JsonToken(JsonToken::NUMBER, oss.str());
}

JsonToken JsonLexer::_maybe_keyword() {
  char tmp[5];
  if (!_stream.read(&tmp[0], 4))
    goto error;
  if (!std::memcmp(tmp, "true", 4))
    return JsonToken(JsonToken::TRUE);
  else if (!std::memcmp(tmp, "null", 4))
    return JsonToken(JsonToken::NULL_);
  if (!_stream.read(&tmp[4], 1))
    goto error;
  if (!std::memcmp(tmp, "false", 5))
    return JsonToken(JsonToken::FALSE);
error:
  throw JsonError("Unexpected character");
}

} // namespace ftjson
