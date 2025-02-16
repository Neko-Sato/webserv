/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_string.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 18:32:51 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/16 19:04:58 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <macros.hpp>

#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

namespace ftpp {

template <typename T>
static std::string __to_string_impl(char const *fmt, T val) {
  std::vector<char> buffer(32);
  for (;;) {
    int res = snprintf(buffer.data(), buffer.size(), fmt, val);
    if (unlikely(res < 0))
      throw std::runtime_error("snprintf failed");
    if (static_cast<size_t>(res) >= buffer.size()) {
      buffer.resize(res + 1);
      continue;
    }
    return buffer.data();
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

std::string to_string(long long val) {
  return __to_string_impl("%lld", val);
}

std::string to_string(unsigned long long val) {
  return __to_string_impl("%llu", val);
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
