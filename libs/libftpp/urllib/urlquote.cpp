/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   urlquote.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:12:07 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/28 20:24:44 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iomanip>
#include <sstream>
#include <string>

namespace ftpp {

std::string urlquote(std::string const &s) {
  std::stringstream ss;
  for (std::string::const_iterator it = s.begin(); it != s.end(); ++it) {
    if (isalnum(*it) || *it == '-' || *it == '_' || *it == '.' || *it == '~')
      ss.put(*it);
    else
      ss << '%' << std::setw(2) << std::hex << std::uppercase
         << static_cast<int>(*it);
  }
  return ss.str();
}

} // namespace ftpp
