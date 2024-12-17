/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonBoolean.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:11:12 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/18 04:20:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <JsonBoolean.hpp>

namespace ftjson {

JsonBoolean::JsonBoolean(bool value) : value(value) {
}

JsonBoolean::JsonBoolean(JsonBoolean const &rhs)
    : JsonValue(rhs), value(rhs.value) {
}

JsonBoolean::~JsonBoolean() {
}

JsonBoolean &JsonBoolean::operator=(JsonBoolean const &rhs) {
  if (this != &rhs) {
    value = rhs.value;
  }
  return *this;
}

JsonBoolean::type JsonBoolean::getType() const {
  return INTEGER;
}

JsonValue *JsonBoolean::copy() const {
  return new JsonBoolean(*this);
}

} // namespace ftjson
