/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonArray.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:11:12 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/18 04:26:47 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <JsonArray.hpp>

namespace ftjson {

JsonArray::JsonArray() {
}

JsonArray::JsonArray(JsonArray const &rhs) : JsonValue(rhs), value(rhs.value) {
}

JsonArray::~JsonArray() {
}

JsonArray &JsonArray::operator=(JsonArray const &rhs) {
  if (this != &rhs) {
    value = rhs.value;
  }
  return *this;
}

JsonArray::type JsonArray::getType() const {
  return ARRAY;
}

JsonValue *JsonArray::copy() const {
  return new JsonArray(*this);
}

} // namespace ftjson
