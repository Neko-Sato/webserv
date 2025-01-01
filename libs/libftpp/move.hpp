/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 10:34:51 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 11:22:43 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

namespace ftpp {

/*

This class instructs the constructor and assignment operator
to perform move operations for lvalues.
For rvalues, please create a swap method.
It resolves by swapping with an invalidated object.

*/

template <typename T> struct remove_reference {
  T &ref;
  explicit remove_reference(T ref);
};

template <typename T> remove_reference<T>::remove_reference(T ref) : ref(ref) {
}

template <typename T> remove_reference<T> move(T &ref) {
  return remove_reference<T>(ref);
}

} // namespace ftpp