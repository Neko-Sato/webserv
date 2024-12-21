/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonToken.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 03:58:26 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/22 04:29:09 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

namespace ftjson {

struct JsonToken {
  enum Type {
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    COMMA,
    COLON,
    STRING,
    NUMBER,
    TRUE,
    FALSE,
    _NULL,
    END,
    INVALID,
  };

  Type type;
  std::string value;

  JsonToken();
  JsonToken(Type type, std::string const &value = "");
  JsonToken(JsonToken const &rhs);
  ~JsonToken();
  JsonToken &operator=(JsonToken const &rhs);
};

} // namespace ftjson
