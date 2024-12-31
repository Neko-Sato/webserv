/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stod.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 21:24:53 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/31 21:41:43 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace ftpp {

double stod(std::string const &str, std::size_t *idx) {
  double result;
  char const *c_str = str.c_str();
  char const *endptr;
  errno = 0;
  result = std::strtod(c_str, const_cast<char **>(&endptr));
  if (errno == ERANGE)
    throw std::out_of_range("stod");
  if (endptr == c_str)
    throw std::invalid_argument("stod");
  if (idx)
    *idx = endptr - c_str;
  return result;
}

} // namespace ftpp