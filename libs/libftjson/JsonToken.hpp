/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonToken.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 03:58:26 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/25 04:19:00 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

namespace ftjson {

class JsonToken {
public:
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
    NULL_,
    END,
    INVALID,
  };

private:
  Type _type;
  std::string _value;

public:
  JsonToken();
  JsonToken(Type type, std::string const &value = "");
  JsonToken(JsonToken const &rhs);
  ~JsonToken();
  JsonToken &operator=(JsonToken const &rhs);

  Type getType() const;
  std::string const &getValue() const;
};

} // namespace ftjson
