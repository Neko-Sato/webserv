/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   html.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 18:20:36 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/11 18:28:11 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <html/html.hpp>
#include <macros.hpp>

#include <sstream>
#include <stdexcept>

namespace ftpp {

std::string htmlEscape(std::string const &str, bool quote) {
  std::stringstream ss;
  for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
    switch (*it) {
    case '&':
      ss << "&amp;";
      break;
    case '<':
      ss << "&lt;";
      break;
    case '>':
      ss << "&gt;";
      break;
    default:
      if (quote) {
        if (*it == '"') {
          ss << "&quot;";
          break;
        } else if (*it == '\'') {
          ss << "&apos;";
          break;
        }
      }
      if (std::iscntrl(*it))
        ss << "&#" << static_cast<int>(*it) << ";";
      else
        ss << *it;
      break;
    }
  }
  return ss.str();
}

std::string htmlUnescape(std::string const &str) {
  UNUSED(str);
  throw std::runtime_error("htmlUnescape not implemented");
}

} // namespace ftpp
