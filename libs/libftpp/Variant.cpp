/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Variant.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 06:43:04 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/26 07:45:26 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Variant.hpp>
#include <cstddef>
#include <typeinfo>

namespace ftpp {

Variant::BaseValue::BaseValue() {
}

Variant::BaseValue::BaseValue(BaseValue const &) {
}

Variant::BaseValue &Variant::BaseValue::operator=(BaseValue const &) {
  return *this;
}

Variant::BaseValue::~BaseValue() {
}

Variant::Variant() : _value(NULL) {
}

Variant::Variant(Variant const &rhs) {
  _value = rhs._value ? rhs._value->copy() : NULL;
}

Variant::~Variant() {
  delete _value;
}

Variant &Variant::operator=(Variant const &rhs) {
  if (this != &rhs) {
    BaseValue *tmp = rhs._value ? rhs._value->copy() : NULL;
    delete _value;
    _value = tmp;
  }
  return *this;
}

std::type_info const &Variant::type() const {
  return _value ? _value->type() : typeid(void);
}

} // namespace ftpp
