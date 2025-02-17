/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stoul.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 21:24:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/17 23:48:46 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace ftpp {

unsigned long stoul(std::string const &str, std::size_t *idx, int base) {
  unsigned long result;
  char const *c_str = str.c_str();
  char const *endptr;
  errno = 0;
  result = std::strtoul(c_str, const_cast<char **>(&endptr), base);
  if (errno == ERANGE)
    throw std::out_of_range("stoul");
  if (endptr == c_str)
    throw std::invalid_argument("stoul");
  if (idx)
    *idx = endptr - c_str;
  return result;
}

} // namespace ftpp
