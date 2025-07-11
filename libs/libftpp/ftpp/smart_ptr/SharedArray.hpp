/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SharedArray.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:26:07 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 07:18:34 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>

namespace ftpp {

// unthread-safe
template <typename T> class SharedArray {
private:
  T *_ptr;
  std::size_t *_count;

public:
  explicit SharedArray(T *ptr = NULL);
  SharedArray(SharedArray const &rhs);
  ~SharedArray();

  SharedArray &operator=(SharedArray const &rhs);

  void reset(T *p = NULL);

  T &operator[](std::size_t i) const;
  operator bool() const;

  T *get() const;
  void swap(SharedArray &rhs) throw();

  std::size_t use_count() const;
};

template <typename T> SharedArray<T>::SharedArray(T *ptr) : _ptr(ptr) {
  try {
    _count = new std::size_t(1);
  } catch (...) {
    delete _ptr;
    throw;
  }
}

template <typename T>
SharedArray<T>::SharedArray(SharedArray const &rhs)
    : _ptr(rhs._ptr), _count(rhs._count) {
  ++*_count;
}

template <typename T> SharedArray<T>::~SharedArray() {
  if (--*_count == 0) {
    delete _ptr;
    delete _count;
  }
}

template <typename T>
SharedArray<T> &SharedArray<T>::operator=(SharedArray const &rhs) {
  if (this->_ptr != rhs._ptr)
    SharedArray(rhs).swap(*this);
  return *this;
}

template <typename T> void SharedArray<T>::reset(T *p) {
  if (_ptr != p)
    SharedArray(p).swap(*this);
}

template <typename T> T &SharedArray<T>::operator[](std::size_t i) const {
  return _ptr[i];
}

template <typename T> SharedArray<T>::operator bool() const {
  return _ptr != NULL;
}

template <typename T> T *SharedArray<T>::get() const {
  return _ptr;
}

template <typename T> void SharedArray<T>::swap(SharedArray &rhs) throw() {
  std::swap(_ptr, rhs._ptr);
  std::swap(_count, rhs._count);
}

template <typename T> std::size_t SharedArray<T>::use_count() const {
  return *_count;
}

} // namespace ftpp

namespace std {

template <typename T>
void swap(ftpp::SharedArray<T> &lhs, ftpp::SharedArray<T> &rhs) throw() {
  lhs.swap(rhs);
}

} // namespace std
