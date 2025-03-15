/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_string.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 18:32:51 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/15 23:35:01 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <macros.hpp>

#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

namespace ftpp {

template <typename T>
static inline std::string __to_string_impl(char const *fmt, T const &val) {
  std::vector<char> buffer(1 << 9);
  for (;;) {
    int res = snprintf(buffer.data(), buffer.size(), fmt, val);
    if (unlikely(res < 0))
      throw std::runtime_error("snprintf failed");
    if (unlikely(buffer.size() < static_cast<size_t>(res)))
      buffer.resize(res);
    else
      return std::string(buffer.data(), res);
  }
}

std::string to_string(int val) {
  return __to_string_impl("%d", val);
}

std::string to_string(unsigned int val) {
  return __to_string_impl("%u", val);
}

std::string to_string(long val) {
  return __to_string_impl("%ld", val);
}

std::string to_string(unsigned long val) {
  return __to_string_impl("%lu", val);
}

std::string to_string(float val) {
  return __to_string_impl("%f", val);
}

std::string to_string(double val) {
  return __to_string_impl("%f", val);
}

std::string to_string(long double val) {
  return __to_string_impl("%Lf", val);
}

} // namespace ftpp
