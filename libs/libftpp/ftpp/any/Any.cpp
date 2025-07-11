/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Any.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 02:17:50 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 07:38:28 by uakizuki         ###   ########.fr       */
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

namespace std {

void swap(ftpp::Any& lhs, ftpp::Any& rhs) throw() {
  lhs.swap(rhs);
}

} // namespace std
