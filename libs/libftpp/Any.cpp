/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Any.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 02:17:50 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/04 21:30:06 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Any.hpp>
#include <cstddef>
#include <macros.hpp>
#include <typeinfo>

#include <algorithm>

namespace ftpp {

Any::BaseValue::BaseValue() {
}

Any::BaseValue::BaseValue(BaseValue const &rhs) {
  UNUSED(rhs);
}

Any::BaseValue &Any::BaseValue::operator=(BaseValue const &rhs) {
  UNUSED(rhs);
  return *this;
}

Any::BaseValue::~BaseValue() {
}

Any::Any() : _value(NULL) {
}

Any::Any(Any const &rhs)
    : _value(rhs._value.get() ? rhs._value->clone() : NULL) {
}

Any::~Any() {
}

Any &Any::operator=(Any const &rhs) {
  if (this != &rhs)
    Any(rhs).swap(*this);
  return *this;
}

void Any::swap(Any &rhs) throw() {
  _value.swap(rhs._value);
}

std::type_info const &Any::type() const {
  return _value.get() ? _value->type() : typeid(void);
}

bool Any::isvalid() const {
  return _value.get() != NULL;
}

} // namespace ftpp
