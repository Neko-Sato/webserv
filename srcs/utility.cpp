/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 21:52:35 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/04 22:43:03 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_string.hpp>

#include <macros.hpp>
#include <stdexcept>

std::size_t parseSize(std::string const &str) {
  std::size_t pos;
  std::size_t result(ftpp::stoul(str, &pos));
  {
    std::size_t unit = 1;
    std::string tmp(str, pos, std::string::npos);
    if (tmp == "k" || tmp == "K")
      unit = 1ul << 10;
    else if (tmp == "m" || tmp == "M")
      unit = 1ul << 20;
    else if (tmp == "g" || tmp == "G")
      unit = 1ul << 30;
    else if (!tmp.empty())
      throw std::runtime_error("invalid size unit");
    if (mul_overflow(result, unit, &result))
      throw std::runtime_error("size overflow");
  }
  return result;
}
