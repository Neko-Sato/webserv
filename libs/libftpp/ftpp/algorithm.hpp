/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 01:28:08 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:41:37 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <cstddef>

namespace ftpp {

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

template <typename C1, typename C2>
bool starts_with(C1 const &input, C2 const &prefix) {
  if (prefix.size() > input.size())
    return false;
  return std::equal(prefix.begin(), prefix.end(), input.begin());
}

template <typename C1, typename C2>
bool ends_with(C1 const &input, C2 const &suffix) {
  if (suffix.size() > input.size())
    return false;
  return std::equal(suffix.rbegin(), suffix.rend(), input.rbegin());
}

} // namespace ftpp
