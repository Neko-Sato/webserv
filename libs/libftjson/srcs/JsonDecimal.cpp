/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonDecimal.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:11:12 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/18 04:20:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <JsonDecimal.hpp>

namespace ftjson {

JsonDecimal::JsonDecimal(double value) : value(value) {
}

JsonDecimal::JsonDecimal(JsonDecimal const &rhs)
    : JsonValue(rhs), value(rhs.value) {
}

JsonDecimal::~JsonDecimal() {
}

JsonDecimal &JsonDecimal::operator=(JsonDecimal const &rhs) {
  if (this != &rhs) {
    value = rhs.value;
  }
  return *this;
}

JsonDecimal::type JsonDecimal::getType() const {
  return INTEGER;
}

JsonValue *JsonDecimal::copy() const {
  return new JsonDecimal(*this);
}

} // namespace ftjson
