/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   html.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 18:20:36 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/26 04:36:00 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <html/html.hpp>
#include <macros.hpp>

#include <sstream>
#include <stdexcept>

namespace ftpp {

std::string htmlEscape(std::string const &str, bool quote) {
  std::ostringstream oss;
  for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
    switch (*it) {
    case '&':
      oss << "&amp;";
      break;
    case '<':
      oss << "&lt;";
      break;
    case '>':
      oss << "&gt;";
      break;
    default:
      if (quote) {
        if (*it == '"') {
          oss << "&quot;";
          break;
        } else if (*it == '\'') {
          oss << "&apos;";
          break;
        }
      }
      if (std::iscntrl(*it))
        oss << "&#" << static_cast<int>(*it) << ";";
      else
        oss << *it;
      break;
    }
  }
  return oss.str();
}

std::string htmlUnescape(std::string const &str) {
  UNUSED(str);
  throw std::runtime_error("htmlUnescape not implemented");
}

} // namespace ftpp
