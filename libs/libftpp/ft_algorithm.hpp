/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_algorithm.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 01:28:08 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/31 17:22:41 by hshimizu         ###   ########.fr       */
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
  bool result = true;
  for (InputIterator it = first; result && it != last; ++it)
    result = result && static_cast<bool>(pred(*it));
  return result;
}

template <typename InputIterator, typename UnaryPredicate>
bool any_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
  bool result = false;
  for (InputIterator it = first; !result && it != last; ++it)
    result = result || static_cast<bool>(pred(*it));
  return result;
}

template <typename InputIterator, typename UnaryPredicate>
bool none_of(InputIterator first, InputIterator last, UnaryPredicate pred) {
  bool result = true;
  for (InputIterator it = first; result && it != last; ++it)
    result = result && !static_cast<bool>(pred(*it));
  return result;
}

} // namespace ftpp