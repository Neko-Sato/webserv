/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonObject.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:11:12 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/18 04:20:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <JsonObject.hpp>

namespace ftjson {

JsonObject::JsonObject() {
}

JsonObject::JsonObject(JsonObject const &rhs)
    : JsonValue(rhs), value(rhs.value) {
}

JsonObject::~JsonObject() {
}

JsonObject &JsonObject::operator=(JsonObject const &rhs) {
  if (this != &rhs) {
    value = rhs.value;
  }
  return *this;
}

JsonObject::type JsonObject::getType() const {
  return OBJECT;
}

JsonValue *JsonObject::copy() const {
  return new JsonObject(*this);
}

} // namespace ftjson
