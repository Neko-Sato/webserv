/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_algorithm.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 01:28:08 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/24 01:28:48 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace ftpp {

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

} // namespace ftpp