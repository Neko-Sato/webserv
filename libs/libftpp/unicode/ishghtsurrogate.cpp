/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ishghtsurrogate.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 04:52:17 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/24 04:56:39 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

namespace ftpp {

bool ishghtsurrogate(unsigned int code) {
  return 0xd800 <= code && code < 0xdc00;
}

} // namespace ftpp
