/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   urlunquote.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:12:07 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/28 20:23:55 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_algorithm.hpp>

#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <string>

namespace ftpp {

std::string urlunquote(std::string const &s) {
  std::stringstream ss;
  for (std::string::const_iterator it = s.begin(); it != s.end(); ++it) {
    switch (*it) {
    case '%':
      if (it + 2 < s.end()) {
        std::string hex(it + 1, it + 3);
        if (ftpp::all_of(hex.begin(), hex.end(), isxdigit)) {
          ss.put(std::strtol(hex.c_str(), NULL, 16));
          it += 2;
          break;
        }
      }
      /* Falls through. */
    default:
      ss.put(*it);
      break;
    }
  }
  return ss.str();
}

} // namespace ftpp
