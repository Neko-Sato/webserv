/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonLexer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 02:25:31 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/19 04:19:01 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <JsonLexer.hpp>

#include <cstring>

namespace ftjson {

JsonLexer::JsonLexer(std::istream &stream) : _stream(stream), _error(false) {
}

JsonLexer::~JsonLexer() {
}

JsonLexer::token JsonLexer::get(std::string &value) {
  value.clear();
  if (_error)
    return ERROR;
  _skip_space();
  char c = _stream.peek();
  if (c == -1)
    return EOF;
  if (c == ',')
    return COMMA;
  else if (c == ':')
    return COLON;
  else if (c == '{')
    return OPEN_CURLYBRACE;
  else if (c == '}')
    return CLOSE_CURLYBRACE;
  else if (c == '[')
    return OPEN_SQUAREBRACKET;
  else if (c == ']')
    return CLOSE_SQUAREBRACKET;
  if (c == '"')
    return _maybe_string(value);
  if (std::isdigit(c) || c == '-')
    return _maybe_number(value);
  _error = true;
  return ERROR;
}

void JsonLexer::_skip_space() {
  while (_stream.peek() == ' ')
    _stream.ignore();
}

JsonLexer::token JsonLexer::_maybe_string(std::string &value) {
  value += _stream.get();
  bool flag = false;
  for (char c; (c = _stream.get()) != -1;) {
    value += c;
    if (!flag) {
      if (c == '\\') {
        flag = true;
        continue;
      }
      if (c == '"')
        return STRING;
    }
    flag = false;
  }
  _error = true;
  return ERROR;
}

JsonLexer::token JsonLexer::_maybe_number(std::string &value) {
  unsigned int flag = 0;
  char c;
  c = _stream.get();
  value += c;
  if (c == '-') {
    if (!std::isdigit(_stream.peek())) {
      _error = true;
      return ERROR;
    }
    c = _stream.get();
    value += c;
  }
  if (c == '0') {
    c = _stream.get();
    value += c;
  }
  while (/* condition */) {
    /* code */
  }
}

} // namespace ftjson
