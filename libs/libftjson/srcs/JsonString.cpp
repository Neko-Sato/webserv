/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonString.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:11:12 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/18 04:20:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <JsonString.hpp>

namespace ftjson {

JsonString::JsonString(std::string const &value) : value(value) {
}

JsonString::JsonString(JsonString const &rhs)
    : JsonValue(rhs), value(rhs.value) {
}

JsonString::~JsonString() {
}

JsonString &JsonString::operator=(JsonString const &rhs) {
  if (this != &rhs) {
    value = rhs.value;
  }
  return *this;
}

JsonString::type JsonString::getType() const {
  return STRING;
}

JsonValue *JsonString::copy() const {
  return new JsonString(*this);
}

} // namespace ftjson
