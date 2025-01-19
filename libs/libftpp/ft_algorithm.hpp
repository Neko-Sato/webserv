/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_algorithm.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 01:28:08 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/19 12:23:19 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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

} // namespace ftpp