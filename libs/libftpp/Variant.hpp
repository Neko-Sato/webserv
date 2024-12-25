/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Variant.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 06:43:04 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/26 07:45:15 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <typeinfo>

namespace ftpp {

class Variant {
private:
  class BaseValue {
  protected:
    BaseValue();
    BaseValue(BaseValue const &);
    virtual BaseValue &operator=(BaseValue const &);

  public:
    virtual ~BaseValue();
    virtual BaseValue *copy() const = 0;
    virtual std::type_info const &type() const = 0;
  };

  template <typename T> class Value : public BaseValue {
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
    BaseValue *copy() const;
    std::type_info const &type() const;
  };

  BaseValue *_value;

public:
  Variant();
  template <typename T> Variant(T const &value);
  Variant(Variant const &rhs);
  ~Variant();
  Variant &operator=(Variant const &rhs);
  template <typename T> Variant &operator=(T const &rhs);
  template <typename T> T &as();
  template <typename T> T const &as() const;
  std::type_info const &type() const;
};

template <typename T> Variant::Value<T>::Value() : _value() {
}

template <typename T> Variant::Value<T>::Value(T const &value) : _value(value) {
}

template <typename T>
Variant::Value<T>::Value(Value const &rhs) : _value(rhs._value) {
}

template <typename T> Variant::Value<T>::~Value() {
}

template <typename T>
typename Variant::Value<T> &Variant::Value<T>::operator=(Value const &rhs) {
  if (this != &rhs) {
    _value = rhs._value;
  }
  return *this;
}

template <typename T> T &Variant::Value<T>::get() {
  return _value;
}

template <typename T> T const &Variant::Value<T>::get() const {
  return _value;
}

template <typename T>
typename Variant::BaseValue *Variant::Value<T>::copy() const {
  return new Value(_value);
}

template <typename T> std::type_info const &Variant::Value<T>::type() const {
  return typeid(T);
}

template <typename T> Variant::Variant(T const &value) {
  _value = new Value<T>(value);
}

template <typename T> Variant &Variant::operator=(T const &rhs) {
  BaseValue *tmp = new Value<T>(rhs);
  delete _value;
  _value = tmp;
  return *this;
}

template <typename T> T &Variant::as() {
  return dynamic_cast<Value<T> &>(*_value).get();
}

template <typename T> T const &Variant::as() const {
  return dynamic_cast<Value<T> const &>(*_value).get();
}

} // namespace ftpp
