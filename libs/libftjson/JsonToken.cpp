/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonToken.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 03:58:26 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/22 05:59:34 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <JsonToken.hpp>

namespace ftjson {

JsonToken::JsonToken() : type(INVALID), value("") {
}

JsonToken::JsonToken(Type type, std::string const &value)
    : type(type), value(value) {
}

JsonToken::JsonToken(JsonToken const &rhs) : type(rhs.type), value(rhs.value) {
}

JsonToken::~JsonToken() {
}

JsonToken &JsonToken::operator=(JsonToken const &rhs) {
  if (this != &rhs) {
    type = rhs.type;
    value = rhs.value;
  }
  return *this;
}

} // namespace ftjson
