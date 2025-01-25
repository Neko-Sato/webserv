/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScopedArray.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:26:07 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/25 08:48:53 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <NonCopyable.hpp>

#include <algorithm>

namespace ftpp {

template <typename T> class ScopedArray : private NonCopyable {
private:
  T *_ptr;

public:
  explicit ScopedArray(T *ptr = NULL);
  ~ScopedArray();

  void reset(T *p = NULL);

  T &operator[](std::size_t i) const;
  operator bool() const;

  T *get() const;
  void swap(ScopedArray &rhs);
};

template <typename T> ScopedArray<T>::ScopedArray(T *ptr) : _ptr(ptr) {
}

template <typename T> ScopedArray<T>::~ScopedArray() {
  delete[] _ptr;
}

template <typename T> void ScopedArray<T>::reset(T *p) {
  if (_ptr != p) {
    delete[] _ptr;
    _ptr = p;
  }
}

template <typename T> T &ScopedArray<T>::operator[](std::size_t i) const {
  return _ptr[i];
}

template <typename T> ScopedArray<T>::operator bool() const {
  return _ptr != NULL;
}

template <typename T> T *ScopedArray<T>::get() const {
  return _ptr;
}

template <typename T> void ScopedArray<T>::swap(ScopedArray &rhs) {
  std::swap(_ptr, rhs._ptr);
}

} // namespace ftpp
