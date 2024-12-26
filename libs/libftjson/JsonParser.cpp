/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 04:30:35 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/26 16:12:54 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Json.hpp>
#include <JsonError.hpp>
#include <JsonParser.hpp>

#include <Variant.hpp>
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

ftpp::Variant JsonParser::_parse() {
  assert(_state.empty());
  assert(_tmp.empty());
  _state.push(STATE_VALUE);
  for (bool flag = true; flag;) {
    JsonToken token = _lexer.nextToken();
    switch (token.getType()) {
    case JsonToken::LEFT_BRACE:
      _case_left_brace();
      break;
    case JsonToken::LEFT_BRACKET:
      _case_left_bracket();
      break;
    case JsonToken::RIGHT_BRACE:
      _case_right_brace();
      break;
    case JsonToken::RIGHT_BRACKET:
      _case_right_bracket();
      break;
    case JsonToken::STRING:
      _case_string(token.getValue());
      break;
    case JsonToken::NUMBER:
      _case_number(token.getValue());
      break;
    case JsonToken::TRUE:
      _case_true();
      break;
    case JsonToken::FALSE:
      _case_false();
      break;
    case JsonToken::NULL_:
      _case_null();
      break;
    case JsonToken::COMMA:
      _case_comma();
      break;
    case JsonToken::COLON:
      _case_colon();
      break;
    default:
      _case_end();
      flag = false;
      break;
    }
  }
  assert(_state.empty());
  ftpp::Variant result = _tmp.top();
  _tmp.pop();
  assert(_tmp.empty());
  return result;
}

void JsonParser::_case_left_brace() {
  switch (_state.top()) {
  case STATE_VALUE:
    _state.top() = STATE_END;
    break;
  case STATE_OBJECT_VALUE:
    _state.top() = STATE_OBJECT_NEXT_OR_END;
    break;
  case STATE_ARRAY_VALUE:
  case STATE_ARRAY_VALUE_OR_END:
    _state.top() = STATE_ARRAY_NEXT_OR_END;
    break;
  default:
    throw JsonError("unexpected token: {");
  }
  _state.push(STATE_OBJECT_KEY_OR_END);
  _tmp.push(Object());
}

void JsonParser::_case_left_bracket() {
  switch (_state.top()) {
  case STATE_VALUE:
    _state.top() = STATE_END;
    break;
  case STATE_OBJECT_VALUE:
    _state.top() = STATE_OBJECT_NEXT_OR_END;
    break;
  case STATE_ARRAY_VALUE_OR_END:
  case STATE_ARRAY_VALUE:
    _state.top() = STATE_ARRAY_NEXT_OR_END;
    break;
  default:
    throw JsonError("unexpected token: [");
  }
  _state.push(STATE_ARRAY_VALUE_OR_END);
  _tmp.push(Array());
}

void JsonParser::_case_right_brace() {
  switch (_state.top()) {
  case STATE_OBJECT_KEY_OR_END:
  case STATE_OBJECT_NEXT_OR_END:
    _state.pop();
    break;
  default:
    throw JsonError("unexpected token: }");
  }
  ftpp::Variant value = _tmp.top();
  _tmp.pop();
  ftpp::Variant key = _tmp.top();
  _tmp.pop();
  _tmp.top().as<Object>()[key.as<String>()] = value;
}

void JsonParser::_case_right_bracket() {
  switch (_state.top()) {
  case STATE_ARRAY_VALUE_OR_END:
  case STATE_ARRAY_NEXT_OR_END:
    _state.pop();
    break;
  default:
    throw JsonError("unexpected token: ]");
  }
  ftpp::Variant value = _tmp.top();
  _tmp.pop();
  _tmp.top().as<Array>().push_back(value);
}

void JsonParser::_case_comma() {
  switch (_state.top()) {
  case STATE_OBJECT_NEXT_OR_END:
    _state.top() = STATE_OBJECT_KEY;
    {
      ftpp::Variant value = _tmp.top();
      _tmp.pop();
      ftpp::Variant key = _tmp.top();
      _tmp.pop();
      _tmp.top().as<Object>()[key.as<String>()] = value;
    }
    break;
  case STATE_ARRAY_NEXT_OR_END:
    _state.top() = STATE_ARRAY_VALUE;
    {
      ftpp::Variant value = _tmp.top();
      _tmp.pop();
      _tmp.top().as<Array>().push_back(value);
    }
    break;
  default:
    throw JsonError("unexpected token: ,");
  }
}

void JsonParser::_case_colon() {
  switch (_state.top()) {
  case STATE_OBJECT_COLON:
    _state.top() = STATE_OBJECT_VALUE;
    break;
  default:
    throw JsonError("unexpected token: :");
  }
}

void JsonParser::_case_string(std::string const &value) {
  switch (_state.top()) {
  case STATE_VALUE:
    _state.top() = STATE_END;
    break;
  case STATE_OBJECT_VALUE:
    _state.top() = STATE_OBJECT_NEXT_OR_END;
    break;
  case STATE_ARRAY_VALUE_OR_END:
  case STATE_ARRAY_VALUE:
    _state.top() = STATE_ARRAY_NEXT_OR_END;
    break;
  case STATE_OBJECT_KEY_OR_END:
  case STATE_OBJECT_KEY:
    _state.top() = STATE_OBJECT_COLON;
    break;
  default:
    throw JsonError("unexpected token: string");
  }
  _tmp.push(_string_dequote(value));
}

void JsonParser::_case_number(std::string const &value) {
  switch (_state.top()) {
  case STATE_VALUE:
    _state.top() = STATE_END;
    break;
  case STATE_OBJECT_VALUE:
    _state.top() = STATE_OBJECT_NEXT_OR_END;
    break;
  case STATE_ARRAY_VALUE_OR_END:
  case STATE_ARRAY_VALUE:
    _state.top() = STATE_ARRAY_NEXT_OR_END;
    break;
  default:
    throw JsonError("unexpected token: number");
  }
  _tmp.push(std::strtod(value.c_str(), NULL));
}

void JsonParser::_case_true() {
  switch (_state.top()) {
  case STATE_VALUE:
    _state.top() = STATE_END;
    break;
  case STATE_OBJECT_VALUE:
    _state.top() = STATE_OBJECT_NEXT_OR_END;
    break;
  case STATE_ARRAY_VALUE_OR_END:
  case STATE_ARRAY_VALUE:
    _state.top() = STATE_ARRAY_NEXT_OR_END;
    break;
  default:
    throw JsonError("unexpected token: true");
  }
  _tmp.push(true);
}

void JsonParser::_case_false() {
  switch (_state.top()) {
  case STATE_VALUE:
    _state.top() = STATE_END;
    break;
  case STATE_OBJECT_VALUE:
    _state.top() = STATE_OBJECT_NEXT_OR_END;
    break;
  case STATE_ARRAY_VALUE_OR_END:
  case STATE_ARRAY_VALUE:
    _state.top() = STATE_ARRAY_NEXT_OR_END;
    break;
  default:
    throw JsonError("unexpected token: false");
  }
  _tmp.push(false);
}

void JsonParser::_case_null() {
  switch (_state.top()) {
  case STATE_VALUE:
    _state.top() = STATE_END;
    break;
  case STATE_OBJECT_VALUE:
    _state.top() = STATE_OBJECT_NEXT_OR_END;
    break;
  case STATE_ARRAY_VALUE_OR_END:
  case STATE_ARRAY_VALUE:
    _state.top() = STATE_ARRAY_NEXT_OR_END;
    break;
  default:
    throw JsonError("unexpected token: null");
  }
  _tmp.push(ftpp::Variant());
}

void JsonParser::_case_end() {
  switch (_state.top()) {
  case STATE_END:
    _state.pop();
    break;
  default:
    throw JsonError("unexpected end");
  }
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

ftpp::Variant JsonParser::parse(char const *str) {
  std::stringstream ss(str);
  return parse(ss);
}

ftpp::Variant JsonParser::parse(std::istream &stream) {
  return JsonParser(stream)._parse();
}

} // namespace ftjson
