/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScopedPtr.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:26:07 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 07:18:03 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftpp/noncopyable/NonCopyable.hpp>

#include <algorithm>

namespace ftpp {

template <typename T> class ScopedPtr : private NonCopyable {
private:
  T *_ptr;

public:
  explicit ScopedPtr(T *ptr = NULL);
  ~ScopedPtr();

  void reset(T *p = NULL);

  T &operator*() const;
  T *operator->() const;
  operator bool() const;

  T *get() const;
  void swap(ScopedPtr &rhs) throw();
};

template <typename T> ScopedPtr<T>::ScopedPtr(T *ptr) : _ptr(ptr) {
}

template <typename T> ScopedPtr<T>::~ScopedPtr() {
  delete _ptr;
}

template <typename T> void ScopedPtr<T>::reset(T *p) {
  if (_ptr != p) {
    delete _ptr;
    _ptr = p;
  }
}

template <typename T> T &ScopedPtr<T>::operator*() const {
  return *_ptr;
}

template <typename T> T *ScopedPtr<T>::operator->() const {
  return _ptr;
}

template <typename T> ScopedPtr<T>::operator bool() const {
  return _ptr != NULL;
}

template <typename T> T *ScopedPtr<T>::get() const {
  return _ptr;
}

template <typename T> void ScopedPtr<T>::swap(ScopedPtr &rhs) throw() {
  std::swap(_ptr, rhs._ptr);
}

} // namespace ftpp

namespace std {

template <typename T>
void swap(ftpp::ScopedPtr<T>& lhs, ftpp::ScopedPtr<T>& rhs) throw() {
  lhs.swap(rhs);
}

} // namespace std
  