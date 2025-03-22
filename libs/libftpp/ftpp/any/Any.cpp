/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Any.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 02:17:50 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/23 00:10:20 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/any/Any.hpp>
#include <ftpp/macros.hpp>

#include <algorithm>
#include <cstddef>
#include <typeinfo>

namespace ftpp {

Any::AnyValue::AnyValue() {
}

Any::AnyValue::AnyValue(AnyValue const &rhs) {
  UNUSED(rhs);
}

Any::AnyValue &Any::AnyValue::operator=(AnyValue const &rhs) {
  UNUSED(rhs);
  return *this;
}

Any::AnyValue::~AnyValue() {
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
