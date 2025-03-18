/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   urlunquote.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:12:07 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:55:34 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/algorithm.hpp>
#include <ftpp/string/string.hpp>

#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <string>

namespace ftpp {

std::string urlunquote(std::string const &s) {
  std::ostringstream oss;
  for (std::string::const_iterator it = s.begin(); it != s.end(); ++it) {
    switch (*it) {
    case '%':
      if (it + 2 < s.end()) {
        std::string hex(it + 1, it + 3);
        if (ftpp::all_of(hex.begin(), hex.end(), isxdigit)) {
          oss.put(stol(hex, NULL, 16));
          it += 2;
          break;
        }
      }
      /* Falls through. */
    default:
      oss.put(*it);
      break;
    }
  }
  return oss.str();
}

} // namespace ftpp
