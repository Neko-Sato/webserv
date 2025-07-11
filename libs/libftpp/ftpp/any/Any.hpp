/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Any.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 02:17:55 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 07:38:25 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftpp/smart_ptr/ScopedPtr.hpp>

#include <cstddef>
#include <typeinfo>

namespace ftpp {

class Any {
private:
  class AnyValue {
  protected:
    AnyValue();
    AnyValue(AnyValue const &rhs);
    AnyValue &operator=(AnyValue const &rhs);

  public:
    virtual ~AnyValue();
    virtual AnyValue *clone() const = 0;
    virtual std::type_info const &type() const = 0;
  };

  template <typename T> class Value : public AnyValue {
  private:
    T _value;

  public:
    Value();
    Value(T const &value);
    Value(Value const &rhs);
    ~Value();
    Value &operator=(Value const &rhs);
    T &get();
    T const &get() const;
    Value *clone() const;
    std::type_info const &type() const;
  };

  ScopedPtr<AnyValue> _value;

public:
  Any();
  Any(Any const &rhs);
  template <typename T> Any(T const &value);
  ~Any();
  Any &operator=(Any const &rhs);
  template <typename T> Any &operator=(T const &rhs);
  void swap(Any &rhs) throw();
  template <typename T> T &as();
  template <typename T> T const &as() const;
  template <typename T> T &as_unsafe();
  template <typename T> T const &as_unsafe() const;
  std::type_info const &type() const;
  template <typename T> bool isType() const;
  bool isvalid() const;
};

template <typename T> Any::Value<T>::Value() : _value() {
}

template <typename T> Any::Value<T>::Value(T const &value) : _value(value) {
}

template <typename T>
Any::Value<T>::Value(Value const &rhs) : _value(rhs._value) {
}

template <typename T> Any::Value<T>::~Value() {
}

template <typename T>
typename Any::Value<T> &Any::Value<T>::operator=(Value const &rhs) {
  if (this != &rhs)
    _value = rhs._value;
  return *this;
}

template <typename T> T &Any::Value<T>::get() {
  return _value;
}

template <typename T> T const &Any::Value<T>::get() const {
  return _value;
}

template <typename T> typename Any::Value<T> *Any::Value<T>::clone() const {
  return new Value(_value);
}

template <typename T> std::type_info const &Any::Value<T>::type() const {
  return typeid(T);
}

template <typename T> Any::Any(T const &value) : _value(new Value<T>(value)) {
}

template <typename T> Any &Any::operator=(T const &rhs) {
  if (isType<T>())
    as_unsafe<T>() = rhs;
  else
    Any(rhs).swap(*this);
  return *this;
}

template <typename T> T &Any::as() {
  return dynamic_cast<Value<T> &>(*_value).get();
}

template <typename T> T const &Any::as() const {
  return dynamic_cast<Value<T> const &>(*_value).get();
}

template <typename T> T &Any::as_unsafe() {
  return static_cast<Value<T> &>(*_value).get();
}

template <typename T> T const &Any::as_unsafe() const {
  return static_cast<Value<T> const &>(*_value).get();
}

template <typename T> bool Any::isType() const {
  return _value->type() == typeid(T);
}

} // namespace ftpp

namespace std {

void swap(ftpp::Any& lhs, ftpp::Any& rhs) throw();

} // namespace std
  