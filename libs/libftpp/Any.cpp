/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Any.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 06:43:04 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/26 16:11:23 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Any.hpp>
#include <cstddef>
#include <typeinfo>

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

std::type_info const &Any::type() const {
  return _value ? _value->type() : typeid(void);
}

bool Any::isvalid() const {
  return _value != NULL;
}

} // namespace ftpp
