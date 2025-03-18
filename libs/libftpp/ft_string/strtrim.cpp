/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strtrim.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 20:53:31 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 10:18:47 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <cctype>
#include <string>

namespace ftpp {

std::string strtrim(std::string const &str) {
  return std::string(
      std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun(::isspace))),
      std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun(::isspace)))
          .base());
}

} // namespace ftpp
