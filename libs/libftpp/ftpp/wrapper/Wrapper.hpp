/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wrapper.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:26:07 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 07:25:11 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>

namespace ftpp {

template <typename T>
struct Cloner {
  T *operator()(T const &rhs) const;
};

template <typename T>
struct PolymorphicCloner {
  T *operator()(T const &rhs) const;
};
 
template <typename T, typename _Cloner=PolymorphicCloner<T> > class Wrapper {
private:
  _Cloner _cloner;
  T *_ptr;

public:
  explicit Wrapper(T *ptr = NULL);
  Wrapper(Wrapper const &rhs);
  Wrapper(T const &value);
  ~Wrapper();

  Wrapper &operator=(Wrapper const &rhs);
  Wrapper &operator=(T const &value);

  void reset(T *p = NULL);

  T &operator*() const;
  T *operator->() const;
  operator bool() const;

  T *get() const;
  void swap(Wrapper &rhs) throw();
};

template <typename T>
T *Cloner<T>::operator()(T const &rhs) const {
  return new T(rhs);
}

template <typename T>
T *PolymorphicCloner<T>::operator()(T const &rhs) const {
  return rhs.clone();
}

template <typename T, typename _Cloner> Wrapper<T, _Cloner>::Wrapper(T *ptr) : _ptr(ptr) {
}

template <typename T, typename _Cloner> Wrapper<T, _Cloner>::Wrapper(Wrapper const &rhs)
    : _ptr(NULL) {
  if (rhs._ptr)
      _ptr = _cloner(*rhs);
}

template <typename T, typename _Cloner>
Wrapper<T, _Cloner>::Wrapper(T const &value) : _ptr(NULL) {
  _ptr = _cloner(value);
}

template <typename T, typename _Cloner> Wrapper<T, _Cloner>::~Wrapper() {
  delete _ptr;
}

template <typename T, typename _Cloner> Wrapper<T, _Cloner> &Wrapper<T, _Cloner>::operator=(
    Wrapper const &rhs) {
  if (this != &rhs)
    Wrapper(rhs).swap(*this);
  return *this;
}

template <typename T, typename _Cloner> Wrapper<T, _Cloner> &Wrapper<T, _Cloner>::operator=(
    T const &value) {
  Wrapper<T, _Cloner>(value).swap(*this);
  return *this;
}

template <typename T, typename _Cloner> void Wrapper<T, _Cloner>::reset(T *p) {
  if (_ptr != p) {
    delete _ptr;
    _ptr = p;
  }
}

template <typename T, typename _Cloner> T &Wrapper<T, _Cloner>::operator*() const {
  return *_ptr;
}

template <typename T, typename _Cloner> T *Wrapper<T, _Cloner>::operator->() const {
  return _ptr;
}

template <typename T, typename _Cloner> Wrapper<T, _Cloner>::operator bool() const {
  return _ptr != NULL;
}

template <typename T, typename _Cloner> T *Wrapper<T, _Cloner>::get() const {
  return _ptr;
}

template <typename T, typename _Cloner> void Wrapper<T, _Cloner>::swap(Wrapper &rhs) throw() {
  std::swap(_ptr, rhs._ptr);
}

} // namespace ftpp

namespace std {

template <typename T, typename _Cloner>
void swap(ftpp::Wrapper<T, _Cloner> &lhs, ftpp::Wrapper<T, _Cloner> &rhs) throw() {
  lhs.swap(rhs);
}
  
} // namespace std
