/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_algorithm.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 01:28:08 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/31 17:29:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstddef>

namespace ftpp {

template <typename T> void swap(T &a, T &b) {
  T tmp = a;
  a = b;
  b = tmp;
}

template <typename T, std::size_t N> void swap(T (&a)[N], T (&b)[N]) {
  for (std::size_t i = 0; i < N; ++i) {
    swap(a[i], b[i]);
  }
}

template <typename InputIterator, typename UnaryPredicate>
bool all_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
  for (InputIterator it = first; it != last; ++it)
    if (!static_cast<bool>(pred(*it)))
      return false;
  return true;
}

template <typename InputIterator, typename UnaryPredicate>
bool any_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
  for (InputIterator it = first; it != last; ++it)
    if (static_cast<bool>(pred(*it)))
      return true;
  return false;
}

template <typename InputIterator, typename UnaryPredicate>
bool none_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
  for (InputIterator it = first; it != last; ++it)
    if (static_cast<bool>(pred(*it)))
      return false;
  return true;
}

} // namespace ftpp