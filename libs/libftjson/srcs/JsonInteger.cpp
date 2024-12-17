/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonInteger.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:11:12 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/18 04:20:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <JsonInteger.hpp>

namespace ftjson {

JsonInteger::JsonInteger(long value) : value(value) {
}

JsonInteger::JsonInteger(JsonInteger const &rhs)
    : JsonValue(rhs), value(rhs.value) {
}

JsonInteger::~JsonInteger() {
}

JsonInteger &JsonInteger::operator=(JsonInteger const &rhs) {
  if (this != &rhs) {
    value = rhs.value;
  }
  return *this;
}

JsonInteger::type JsonInteger::getType() const {
  return INTEGER;
}

JsonValue *JsonInteger::copy() const {
  return new JsonInteger(*this);
}

} // namespace ftjson
