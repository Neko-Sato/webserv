/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strtrim.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 20:53:31 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/25 21:22:00 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <cctype>
#include <string>

namespace ftpp {

struct notSpace {
  bool operator()(int c) const {
    return !std::isspace(c);
  };
};

std::string strtrim(std::string const &str) {
  ;
  return std::string(std::find_if(str.begin(), str.end(), notSpace()),
                     std::find_if(str.rbegin(), str.rend(), notSpace()).base());
}

} // namespace ftpp
