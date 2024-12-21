/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonNumber.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:11:12 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/18 04:20:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <JsonNumber.hpp>

namespace ftjson {

JsonNumber::JsonNumber(double value) : value(value) {
}

JsonNumber::JsonNumber(JsonNumber const &rhs)
    : JsonValue(rhs), value(rhs.value) {
}

JsonNumber::~JsonNumber() {
}

JsonNumber &JsonNumber::operator=(JsonNumber const &rhs) {
  if (this != &rhs) {
    value = rhs.value;
  }
  return *this;
}

JsonNumber::type JsonNumber::getType() const {
  return NUMBER;
}

JsonValue *JsonNumber::copy() const {
  return new JsonNumber(*this);
}

} // namespace ftjson
