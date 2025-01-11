/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Any.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 02:17:50 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/11 13:57:53 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Any.hpp>
#include <cstddef>
#include <typeinfo>

#include <algorithm>

namespace ftpp {

Any::BaseValue::BaseValue() {
}

Any::BaseValue::BaseValue(BaseValue const &) {
}

Any::BaseValue &Any::BaseValue::operator=(BaseValue const &) {
  return *this;
}

Any::BaseValue::~BaseValue() {
}

Any::Any() : _value(NULL) {
}

Any::Any(Any const &rhs) {
  _value = rhs._value ? rhs._value->copy() : NULL;
}

Any::~Any() {
  delete _value;
}

Any &Any::operator=(Any const &rhs) {
  if (this != &rhs) {
    BaseValue *tmp = rhs._value ? rhs._value->copy() : NULL;
    delete _value;
    _value = tmp;
  }
  return *this;
}

void Any::swap(Any &rhs) {
  if (this != &rhs)
    std::swap(_value, rhs._value);
}

std::type_info const &Any::type() const {
  return _value ? _value->type() : typeid(void);
}

bool Any::isvalid() const {
  return _value != NULL;
}

} // namespace ftpp
