/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stoi.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 21:24:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:55:34 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/string/string.hpp>

#include <limits>
#include <stdexcept>

namespace ftpp {

int stoi(std::string const &str, std::size_t *idx, int base) {
  long result = ftpp::stol(str, idx, base);
  if (result < std::numeric_limits<int>::min() ||
      result > std::numeric_limits<int>::max())
    throw std::out_of_range("stoi");
  return static_cast<int>(result);
}

} // namespace ftpp
