/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Optional.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:27:54 by uakizuki          #+#    #+#             */
/*   Updated: 2025/07/12 07:47:51 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstddef>

namespace ftpp {

template <typename T>
class Optional {
private:
  bool _enable;
  char _data[sizeof(T)];

public:
  typedef T value_type;

  Optional();
  Optional(Optional const &rhs);
  Optional(T const &value);
  ~Optional();

  Optional &operator=(Optional const &rhs);
  Optional &operator=(T const &value);
  
  void swap(Optional &rhs) throw();

  operator bool() const;
  void *ptr();
  void activate();
  void destroy();

  T &operator*();
  T const &operator*() const;
  T *operator->();
  T const *operator->() const;
};

template <typename T>
Optional<T>::Optional()
  : _enable(false) {
}

template <typename T>
Optional<T>::Optional(Optional const &rhs)
  : _enable(rhs._enable) {
  if (_enable)
    new (_data) T(*rhs);
}

template <typename T>
Optional<T>::Optional(T const &value)
  : _enable(true) {
  new (_data) T(value);
}

template <typename T>
Optional<T>::~Optional() {
  if (_enable)
    (*this)->~T();
}

template <typename T>
Optional<T> &Optional<T>::operator=(Optional const &rhs) {
  if (this != &rhs) {
    if (_enable && rhs._enable)
      *(*this) = *rhs;
    else if (!_enable && rhs._enable)
      new (_data) T(*rhs);
    else if (_enable && !rhs._enable)
      (*this)->~T();
    _enable = rhs._enable;
  }
  return *this;
}

template <typename T>
Optional<T> &Optional<T>::operator=(T const &value) {
  if (_enable)
    *(*this) = value;
  else
    new (_data) T(value);
  _enable = true;
  return *this;
}

template <typename T>
void Optional<T>::swap(Optional &rhs) throw() {
  if(this == &rhs)
    return;
  if (_enable && rhs._enable) {
    std::swap(*(*this), *rhs);
  } else if (_enable && !rhs._enable) {
    new (rhs._data) T(*(*this));
    (*this)->~T();
  } else if (!_enable && rhs._enable) {
    new (_data) T(*rhs);
    rhs->~T();
  }
  std::swap(_enable, rhs._enable);
}

template <typename T>
Optional<T>::operator bool() const {
  return _enable;
}

template <typename T>
void *Optional<T>::ptr() {
  return _data;
}

template <typename T>
void Optional<T>::activate() {
  _enable = true;
}

template <typename T>
T &Optional<T>::operator*() {
  return *reinterpret_cast<T *>(_enable ? _data : NULL);
}

template <typename T>
T const &Optional<T>::operator*() const {
  return *reinterpret_cast<T const *>(_enable ? _data : NULL);
}

template <typename T>
T *Optional<T>::operator->() {
  return reinterpret_cast<T *>(_enable ? _data : NULL);
}

template <typename T>
T const *Optional<T>::operator->() const {
  return reinterpret_cast<T const *>(_enable ? _data : NULL);
}

template <typename T>
void Optional<T>::destroy() {
  if (_enable)
    (*this)->~T();
  _enable = false;
}

} // namespace ftpp

namespace std {

template <typename T>
void swap(ftpp::Optional<T>& lhs, ftpp::Optional<T>& rhs) throw() {
  lhs.swap(rhs);
}

} // namespace std
