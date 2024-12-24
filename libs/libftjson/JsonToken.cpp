/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonToken.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 03:58:26 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/25 04:19:08 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <JsonToken.hpp>

namespace ftjson {

JsonToken::JsonToken() : _type(INVALID) {
}

JsonToken::JsonToken(Type type, std::string const &value)
    : _type(type), _value(value) {
}

JsonToken::JsonToken(JsonToken const &rhs)
    : _type(rhs._type), _value(rhs._value) {
}

JsonToken::~JsonToken() {
}

JsonToken &JsonToken::operator=(JsonToken const &rhs) {
  if (this != &rhs) {
    _type = rhs._type;
    _value = rhs._value;
  }
  return *this;
}

JsonToken::Type JsonToken::getType() const {
  return _type;
}

std::string const &JsonToken::getValue() const {
  return _value;
}

} // namespace ftjson
