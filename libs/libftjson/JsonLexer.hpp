/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonLexer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 02:25:31 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/19 04:10:43 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <istream>

namespace ftjson {

class JsonLexer {
private:
  std::istream &_stream;
  bool _error;

  JsonLexer();
  JsonLexer(JsonLexer const &rhs);
  JsonLexer &operator=(JsonLexer const &rhs);

public:
  JsonLexer(std::istream &stream);
  ~JsonLexer();

  enum token {
    ERROR,
    EOF,
    COMMA,
    COLON,
    OPEN_CURLYBRACE,
    CLOSE_CURLYBRACE,
    OPEN_SQUAREBRACKET,
    CLOSE_SQUAREBRACKET,
    STRING,
    NUMBER,
    BOOLEAN,
    _NULL,
  };

  token get(std::string &value);

private:
  void _skip_space();
  token _maybe_string(std::string &value);
  token _maybe_number(std::string &value);
};

} // namespace ftjson
