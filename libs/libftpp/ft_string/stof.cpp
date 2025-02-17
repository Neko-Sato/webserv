/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stof.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 21:24:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/17 23:48:36 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace ftpp {

float stof(std::string const &str, std::size_t *idx) {
  float result;
  char const *c_str = str.c_str();
  char const *endptr;
  errno = 0;
  result = std::strtof(c_str, const_cast<char **>(&endptr));
  if (errno == ERANGE)
    throw std::out_of_range("stof");
  if (endptr == c_str)
    throw std::invalid_argument("stof");
  if (idx)
    *idx = endptr - c_str;
  return result;
}

} // namespace ftpp
