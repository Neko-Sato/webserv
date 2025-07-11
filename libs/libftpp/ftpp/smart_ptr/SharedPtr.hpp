/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SharedPtr.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:26:07 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 07:18:51 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>

namespace ftpp {

// unthread-safe
template <typename T> class SharedPtr {
private:
  T *_ptr;
  std::size_t *_count;

public:
  explicit SharedPtr(T *ptr = NULL);
  SharedPtr(SharedPtr const &rhs);
  ~SharedPtr();

  SharedPtr &operator=(SharedPtr const &rhs);

  void reset(T *p = NULL);

  T &operator*() const;
  T *operator->() const;
  operator bool() const;

  T *get() const;
  void swap(SharedPtr &rhs) throw();

  std::size_t use_count() const;
};

template <typename T> SharedPtr<T>::SharedPtr(T *ptr) : _ptr(ptr) {
  try {
    _count = new std::size_t(1);
  } catch (...) {
    delete _ptr;
    throw;
  }
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr const &rhs)
    : _ptr(rhs._ptr), _count(rhs._count) {
  ++*_count;
}

template <typename T> SharedPtr<T>::~SharedPtr() {
  if (--*_count == 0) {
    delete _ptr;
    delete _count;
  }
}

template <typename T>
SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr const &rhs) {
  if (this->_ptr != rhs._ptr)
    SharedPtr(rhs).swap(*this);
  return *this;
}

template <typename T> void SharedPtr<T>::reset(T *p) {
  if (_ptr != p)
    SharedPtr(p).swap(*this);
}

template <typename T> T &SharedPtr<T>::operator*() const {
  return *_ptr;
}

template <typename T> T *SharedPtr<T>::operator->() const {
  return _ptr;
}

template <typename T> SharedPtr<T>::operator bool() const {
  return _ptr != NULL;
}

template <typename T> T *SharedPtr<T>::get() const {
  return _ptr;
}

template <typename T> void SharedPtr<T>::swap(SharedPtr &rhs) throw() {
  std::swap(_ptr, rhs._ptr);
  std::swap(_count, rhs._count);
}

template <typename T> std::size_t SharedPtr<T>::use_count() const {
  return *_count;
}

} // namespace ftpp

namespace std {

template <typename T>
void swap(ftpp::SharedPtr<T> &lhs, ftpp::SharedPtr<T> &rhs) throw() {
  lhs.swap(rhs);
}

} // namespace std
  