/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stoi.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 21:24:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/04 01:30:13 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_string.hpp>

#include <limits>
#include <stdexcept>

namespace ftpp {

int stoi(std::string const &str, std::size_t *idx, int base) {
  long result = stol(str, idx, base);
  if (result < std::numeric_limits<int>::min() ||
      result > std::numeric_limits<int>::max())
    throw std::out_of_range("stoi");
  return static_cast<int>(result);
}

} // namespace ftpp