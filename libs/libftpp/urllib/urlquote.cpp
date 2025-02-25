/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   urlquote.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:12:07 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/26 04:36:58 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iomanip>
#include <sstream>
#include <string>

namespace ftpp {

std::string urlquote(std::string const &s) {
  std::ostringstream oss;
  for (std::string::const_iterator it = s.begin(); it != s.end(); ++it) {
    if (isalnum(*it) || *it == '-' || *it == '_' || *it == '.' || *it == '~')
      oss.put(*it);
    else
      oss << '%' << std::setw(2) << std::hex << std::uppercase
          << static_cast<int>(*it & 0xFF);
  }
  return oss.str();
}

} // namespace ftpp
