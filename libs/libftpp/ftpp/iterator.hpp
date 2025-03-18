/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 12:28:47 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:37:22 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iterator>

namespace ftpp {

template <typename InputIterator>
InputIterator
next(InputIterator it,
     typename std::iterator_traits<InputIterator>::difference_type n = 1) {
  std::advance(it, n);
  return it;
}

template <typename InputIterator>
InputIterator
prev(InputIterator it,
     typename std::iterator_traits<InputIterator>::difference_type n = 1) {
  std::advance(it, -n);
  return it;
}

} // namespace ftpp
