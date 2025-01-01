/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 10:34:51 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 13:08:15 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace ftpp {

/*

This class instructs the constructor and assignment operator
to perform move operations for lvalues.
For rvalues, please create a swap method.
It resolves by swapping with an invalidated object.

*/

template <typename T> struct transfer {
  T &ref;
  explicit transfer(T &ref);
};

template <typename T> transfer<T>::transfer(T &ref) : ref(ref) {
}

template <typename T> transfer<T> move(T &ref) {
  return transfer<T>(ref);
}

} // namespace ftpp