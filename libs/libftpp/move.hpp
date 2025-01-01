/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 10:34:51 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 12:29:33 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

namespace ftpp {

/*

This class instructs the constructor and assignment operator
to perform move operations for lvalues.
For rvalues, please create a swap method.
It resolves by swapping with an invalidated object.

*/

template <typename T> struct rm_ref {
  T &ref;
  explicit rm_ref(T ref);
};

template <typename T> rm_ref<T>::rm_ref(T ref) : ref(ref) {
}

template <typename T> rm_ref<T> move(T &ref) {
  return rm_ref<T>(ref);
}

} // namespace ftpp