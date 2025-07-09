/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CopyablePtr.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:26:07 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/09 20:18:23 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>

namespace ftpp {

template <typename T, typename _Cloner> class _CopyablePtr {
private:
  _Cloner _cloner;
  T *_ptr;

public:
  explicit _CopyablePtr(T *ptr = NULL);
  _CopyablePtr(_CopyablePtr const &rhs);
  ~_CopyablePtr();

  _CopyablePtr &operator=(_CopyablePtr const &rhs);

  void reset(T *p = NULL);

  T &operator*() const;
  T *operator->() const;
  operator bool() const;

  T *get() const;
  void swap(_CopyablePtr &rhs) throw();
};

template <typename T>
struct DefaultCloner {
  T *operator()(T const &rhs) const;
};

template <typename T>
struct PolymorphicCloner {
  T *operator()(T const &rhs) const;
};

template <typename T>
struct CopyablePtr {
  typedef typename _CopyablePtr<T, DefaultCloner<T> > Default;
  typedef typename _CopyablePtr<T, PolymorphicCloner<T> > Polymorphic;
};

template <typename T>
T *DefaultCloner<T>::operator()(T const &rhs) const {
  return new T(rhs);
}

template <typename T>
T *PolymorphicCloner<T>::operator()(T const &rhs) const {
  return rhs.clone();
}

template <typename T, typename _Cloner> _CopyablePtr<T, _Cloner>::_CopyablePtr(T *ptr) : _ptr(ptr) {
}

template <typename T, typename _Cloner> _CopyablePtr<T, _Cloner>::_CopyablePtr(_CopyablePtr const &rhs)
    : _ptr(NULL) {
  if (rhs._ptr)
      _ptr = _cloner(*rhs);
}

template <typename T, typename _Cloner> _CopyablePtr<T, _Cloner>::~_CopyablePtr() {
  delete _ptr;
}

template <typename T, typename _Cloner> _CopyablePtr<T, _Cloner> &_CopyablePtr<T, _Cloner>::operator=(
    _CopyablePtr const &rhs) {
  if (this != &rhs)
    _CopyablePtr(rhs).swap(*this);
  return *this;
}

template <typename T, typename _Cloner> void _CopyablePtr<T, _Cloner>::reset(T *p) {
  if (_ptr != p) {
    delete _ptr;
    _ptr = p;
  }
}

template <typename T, typename _Cloner> T &_CopyablePtr<T, _Cloner>::operator*() const {
  return *_ptr;
}

template <typename T, typename _Cloner> T *_CopyablePtr<T, _Cloner>::operator->() const {
  return _ptr;
}

template <typename T, typename _Cloner> _CopyablePtr<T, _Cloner>::operator bool() const {
  return _ptr != NULL;
}

template <typename T, typename _Cloner> T *_CopyablePtr<T, _Cloner>::get() const {
  return _ptr;
}

template <typename T, typename _Cloner> void _CopyablePtr<T, _Cloner>::swap(_CopyablePtr &rhs) throw() {
  std::swap(_ptr, rhs._ptr);
}

} // namespace ftpp
