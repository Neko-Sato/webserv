/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Json.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 23:19:16 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/18 04:58:42 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Json.hpp>
#include <JsonValue.hpp>

#include <sstream>

namespace ftjson {

Json::Json(JsonValue *value) : _value(value) {
}

Json::Json(JsonValue const &value) : _value(value.copy()) {
}

Json::Json(Json const &rhs) : _value(rhs._value->copy()) {
}

Json::~Json() {
  delete _value;
}

Json &Json::operator=(Json const &rhs) {
  if (this != &rhs) {
    JsonValue *tmp;
    tmp = rhs._value->copy();
    delete _value;
    _value = tmp;
  }
  return *this;
}

JsonValue &Json::getValue() {
  return *_value;
}

JsonValue const &Json::getValue() const {
  return *_value;
}

} // namespace ftjson
