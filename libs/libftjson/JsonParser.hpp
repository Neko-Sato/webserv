/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 04:30:35 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/28 02:33:57 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <JsonLexer.hpp>

#include <Variant.hpp>

#include <stack>

namespace ftjson {

class JsonParser {
private:
  JsonLexer _lexer;

  enum state {
    STATE_END,
    STATE_INVALID,
    STATE_EOF,
    STATE_VALUE,
    STATE_OBJECT_KEY_OR_END,
    STATE_OBJECT_KEY,
    STATE_OBJECT_COLON,
    STATE_OBJECT_VALUE,
    STATE_OBJECT_NEXT_OR_END,
    STATE_ARRAY_VALUE_OR_END,
    STATE_ARRAY_VALUE,
    STATE_ARRAY_NEXT_OR_END,
  };

  std::stack<state> _state;
  state _current_state;
  std::stack<ftpp::Variant> _tmp;

  JsonParser(std::istream &stream);
  ~JsonParser();

  ftpp::Variant _parse();

  void _transition(JsonToken const &token);

  void _case_left_brace();
  void _case_left_bracket();
  void _case_right_brace();
  void _case_right_bracket();
  void _case_comma();
  void _case_colon();
  void _case_string(std::string const &value);
  void _case_number(std::string const &value);
  void _case_true();
  void _case_false();
  void _case_null();
  void _case_end();

  void _insert_object();
  void _insert_array();

  static std::string _string_dequote(std::string const &str);

  JsonParser(JsonParser const &rhs);
  JsonParser &operator=(JsonParser const &rhs);

public:
  static ftpp::Variant parse(char const *str);
  static ftpp::Variant parse(std::istream &stream);
};

} // namespace ftjson
