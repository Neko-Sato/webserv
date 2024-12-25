/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Variant.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 06:43:04 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/26 07:39:54 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstddef>
#include <typeinfo>

namespace ftpp {

class Variant {
private:
  class BaseValue {
  protected:
    BaseValue() {};
    BaseValue(BaseValue const &) {};
    virtual BaseValue &operator=(BaseValue const &) {};

  public:
    virtual ~BaseValue() {};
    virtual BaseValue *copy() const = 0;
    virtual std::type_info const &type() const = 0;
  };

  template <typename T> class Value : public BaseValue {
  private:
    T _value;

  public:
    Value() {};
    Value(T const &value) : _value(value) {};
    Value(Value const &rhs) : _value(rhs._value) {};
    ~Value() {};
    Value &operator=(Value const &rhs) {
      if (this != &rhs) {
        _value = rhs._value;
      }
      return *this;
    };
    T &get() {
      return _value;
    };
    T const &get() const {
      return _value;
    };
    BaseValue *copy() const {
      return new Value(_value);
    };
    std::type_info const &type() const {
      return typeid(T);
    };
  };

  BaseValue *_value;

public:
  Variant() : _value(NULL) {};

  template <typename T> Variant(T const &value) {
    _value = new Value<T>(value);
  };

  Variant(Variant const &rhs) {
    _value = rhs._value ? rhs._value->copy() : NULL;
  };

  ~Variant() {
    delete _value;
  };

  Variant &operator=(Variant const &rhs) {
    if (this != &rhs) {
      BaseValue *tmp = rhs._value ? rhs._value->copy() : NULL;
      delete _value;
      _value = tmp;
    }
    return *this;
  };

  template <typename T> Variant &operator=(T const &rhs) {
    BaseValue *tmp = new Value<T>(rhs);
    delete _value;
    _value = tmp;
    return *this;
  };

  template <typename T> T &as() {
    return dynamic_cast<Value<T> &>(*_value).get();
  };

  template <typename T> T const &as() const {
    return dynamic_cast<Value<T> const &>(*_value).get();
  };

  std::type_info const &type() const {
    return _value ? _value->type() : typeid(void);
  };
};

} // namespace ftpp
