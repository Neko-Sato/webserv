/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Array.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 14:38:17 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 07:14:20 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
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
  void swap(Array &rhs) throw();
};

template <typename T, std::size_t N> Array<T, N>::Array() {
}

template <typename T, std::size_t N> Array<T, N>::Array(T const *data) {
  std::copy(data, &data[N], _data);
}

template <typename T, std::size_t N> Array<T, N>::Array(Array const &rhs) {
  std::copy(rhs._data, &rhs._data[N], _data);
}

template <typename T, std::size_t N>
Array<T, N> &Array<T, N>::operator=(Array const &rhs) {
  std::copy(rhs._data, &rhs._data[N], _data);
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

template <typename T, std::size_t N>
void Array<T, N>::swap(Array &rhs) throw() {
  std::swap_ranges(_data, &_data[N], rhs._data);
}

} // namespace ftpp

namespace std {

template <typename T, std::size_t N>
void swap(ftpp::Array<T, N>& lhs, ftpp::Array<T, N>& rhs) throw() {
  lhs.swap(rhs);
}

} // namespace std
  