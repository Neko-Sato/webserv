/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Array.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 14:38:17 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/11 15:03:33 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstddef>

namespace ftpp {

template <typename T, std::size_t N> class Array {
private:
  T _data[N];

public:
  Array();
  Array(T const *data);
  Array(Array const &rhs);
  Array &operator=(Array const &rhs);
  ~Array();

  T &operator[](std::size_t pos);
  T const &operator[](std::size_t pos) const;
  std::size_t size() const;
  T *data();
  T const *data() const;
  void swap(Array &rhs);
};

template <typename T, std::size_t N> Array<T, N>::Array() {
}

template <typename T, std::size_t N> Array<T, N>::Array(T const *data) {
  for (std::size_t i = 0; i < N; i++)
    _data[i] = data[i];
}

template <typename T, std::size_t N> Array<T, N>::Array(Array const &rhs) {
  for (size_t i = 0; i < N; i++)
    _data[i] = rhs._data[i];
}

template <typename T, std::size_t N>
Array<T, N> &Array<T, N>::operator=(Array const &rhs) {
  for (size_t i = 0; i < N; i++)
    _data[i] = rhs._data[i];
  return *this;
}

template <typename T, std::size_t N> Array<T, N>::~Array() {
}

template <typename T, std::size_t N>
T &Array<T, N>::operator[](std::size_t pos) {
  return _data[pos];
}

template <typename T, std::size_t N>
T const &Array<T, N>::operator[](std::size_t pos) const {
  return _data[pos];
}

template <typename T, std::size_t N> std::size_t Array<T, N>::size() const {
  return N;
}

template <typename T, std::size_t N> T *Array<T, N>::data() {
  return _data;
}

template <typename T, std::size_t N> T const *Array<T, N>::data() const {
  return _data;
}

template <typename T, std::size_t N> void Array<T, N>::swap(Array &rhs) {
  for (std::size_t i = 0; i < N; i++)
    std::swap(_data[i], rhs._data[i]);
}

} // namespace ftpp
