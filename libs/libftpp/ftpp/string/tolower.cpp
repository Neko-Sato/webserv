/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tolower.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 22:49:25 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/26 22:53:19 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <cctype>
#include <string>

namespace ftpp {

std::string tolower(std::string const &str) {
  std::string ret(str);
  std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
  return ret;
}

} // namespace ftpp
