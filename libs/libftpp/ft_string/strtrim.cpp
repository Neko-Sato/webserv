/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strtrim.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 20:53:31 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/04 21:11:52 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <string>

namespace ftpp {

std::string strtrim(std::string const &str) {
  std::string::const_iterator first = str.begin();
  for (; first != str.end() && isspace(*first); ++first)
    ;
  std::string::const_reverse_iterator last = str.rbegin();
  for (; last != str.rend() && isspace(*last); ++last)
    ;
  return std::string(first, last.base());
}

} // namespace ftpp
