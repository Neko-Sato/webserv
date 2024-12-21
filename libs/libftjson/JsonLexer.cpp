/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonLexer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 02:25:31 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/22 06:35:30 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <JsonLexer.hpp>

#include <cctype>
#include <cstring>

namespace ftjson {

JsonLexer::JsonLexer(std::istream &stream) : _stream(stream), _error(false) {
}

JsonLexer::~JsonLexer() {
}

JsonToken JsonLexer::nextToken() {
  if (_error)
    return JsonToken(JsonToken::INVALID);
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
    char tmp[8];
    if (!_stream.read(tmp, 4))
      goto invalid;
    if (!std::strncmp(tmp, "true", 4))
      return JsonToken(JsonToken::TRUE);
    else if (!std::strncmp(tmp, "null", 4))
      return JsonToken(JsonToken::_NULL);
    if (!_stream.read(&tmp[4], 1))
      goto invalid;
    if (!std::strncmp(tmp, "false", 5))
      return JsonToken(JsonToken::FALSE);
  }
invalid:
  _error = true;
  return JsonToken(JsonToken::INVALID);
}

void JsonLexer::_skip_space() {
  while (std::isspace(_stream.peek()))
    _stream.ignore();
}

JsonToken JsonLexer::_maybe_string() {
  std::stringstream ss;
  bool flag = false;
  ss.put(_stream.get());
  for (char c; (c = _stream.peek()) != -1;) {
    if (!std::isprint(c))
      goto invalid;
    ss.put(_stream.get());
    if (!flag) {
      if (c == '\\') {
        flag = true;
        continue;
      }
      if (c == '"')
        return JsonToken(JsonToken::STRING, ss.str());
    }
    flag = false;
  }
invalid:
  _error = true;
  return JsonToken(JsonToken::INVALID);
}

JsonToken JsonLexer::_maybe_number() {
  std::stringstream ss;
  if (_stream.peek() == '-') {
    ss.put(_stream.get());
    if (_stream.eof() || !std::isdigit(_stream.peek()))
      goto invalid;
  }
  for (char c; (c = _stream.peek()) != -1;) {
    if (!std::isdigit(c))
      break;
    ss.put(c);
    _stream.ignore();
    if (c == '0')
      break;
  }
  if (_stream.peek() == '.') {
    ss.put(_stream.get());
    for (char c; (c = _stream.peek()) != -1; _stream.ignore()) {
      if (!std::isdigit(c))
        break;
      ss.put(c);
    }
  }
  if (std::tolower(_stream.peek()) == 'e') {
    ss.put(_stream.get());
    if (std::memchr("+-", _stream.peek(), 2)) {
      ss.put(_stream.get());
      if (_stream.eof() || !std::isdigit(_stream.peek()))
        goto invalid;
    }
    for (char c; (c = _stream.peek()) != -1; _stream.ignore()) {
      if (!std::isdigit(c))
        break;
      ss.put(c);
    }
    if (std::isdigit(_stream.peek()))
      goto invalid;
    return JsonToken(JsonToken::NUMBER, ss.str());
  }
invalid:
  _error = true;
  return JsonToken(JsonToken::INVALID);
}

} // namespace ftjson
