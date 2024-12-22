/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 04:30:35 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/22 09:45:15 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <JsonParser.hpp>

#include <JsonArray.hpp>
#include <JsonBoolean.hpp>
#include <JsonNumber.hpp>
#include <JsonObject.hpp>
#include <JsonString.hpp>

#include <cassert>
#include <cstdlib>
#include <istream>
#include <sstream>

namespace ftjson {

JsonParser::JsonParser(std::istream &stream) : _lexer(stream) {
}

JsonParser::~JsonParser() {
}

/*
どうしたらいいのか、、、
*/
Json JsonParser::_parse() {
//   JsonToken token;
  Json result;
//   switch (token.type) {
//   case JsonToken::LEFT_BRACE:
//     result = _parse_object();
//     break;
//   case JsonToken::LEFT_BRACKET:
//     result = _parse_array();
//     break;
//   case JsonToken::STRING:
//     result = Json(JsonString(_string_dequote(token.value)));
//     break;
//   case JsonToken::NUMBER:
//     result = Json(JsonNumber(std::strtod(token.value.c_str(), NULL)));
//     break;
//   case JsonToken::TRUE:
//   case JsonToken::FALSE:
//     result = Json(JsonBoolean(token.type == JsonToken::TRUE));
//     break;
//   case JsonToken::_NULL:
//     break;
//   default:
//     throw std::runtime_error("unexpected token");
//   }
  return result;
}

std::string JsonParser::_string_dequote(std::string const &str) {
  std::stringstream ss;
  std::string::const_iterator it = str.begin();
  assert(it != str.end() && *it == '"');
  ++it;
  for (; it != str.end(); ++it) {
    if (*it == '"') {
      ++it;
      break;
    } else if (*it == '\\') {
      ++it;
      assert(it != str.end());
      switch (*it) {
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
        throw std::runtime_error("unicode escape is not supported");
      default:
        ss.put(*it);
        break;
      }
    } else
      ss.put(*it);
  }
  assert(it == str.end());
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
