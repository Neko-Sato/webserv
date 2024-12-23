/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   islowsurrogate.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 04:52:17 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/24 04:56:47 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

namespace ftpp {

bool islowsurrogate(unsigned int code) {
  return 0xdc00 <= code && code < 0xe000;
}

} // namespace ftpp
