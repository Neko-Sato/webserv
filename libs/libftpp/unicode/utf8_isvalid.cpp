/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utf8_isvalid.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 12:26:24 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/24 02:35:29 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unicode/utf8.hpp>

namespace ftpp {

bool utf8_isvalid(char const *buffer, std::size_t size) {
  unsigned char const *p = reinterpret_cast<unsigned char const *>(buffer);
  int len = utf8_length(p[0]);
  if (len < 0 || static_cast<std::size_t>(len) < size)
    return false;
  switch (len) {
  case 1:
    return p[0] <= 0x7F;
  case 2:
    return p[1] >= 0x80 && p[1] <= 0xBF;
  case 3:
    return (p[1] >= 0x80 && p[1] <= 0xBF) && (p[2] >= 0x80 && p[2] <= 0xBF);
  case 4:
    return (p[1] >= 0x80 && p[1] <= 0xBF) && (p[2] >= 0x80 && p[2] <= 0xBF) &&
           (p[3] >= 0x80 && p[3] <= 0xBF);
  default:
    return false;
  }
}

} // namespace ftpp
