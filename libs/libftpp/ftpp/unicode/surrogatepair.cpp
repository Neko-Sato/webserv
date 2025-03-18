/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surrogatepair.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 04:52:17 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:55:34 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/unicode/UnicodeError.hpp>
#include <ftpp/unicode/surrogate.hpp>

namespace ftpp {

unsigned int surrogatepair(unsigned int high, unsigned int low) {
  if (!ishghtsurrogate(high) || !islowsurrogate(low))
    throw UnicodeError("invalid surrogate pair");
  return ((high & 0x3FF) << 10) + (low & 0x3FF) + 0x10000;
}

} // namespace ftpp
