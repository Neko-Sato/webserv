/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messyformat.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:55:26 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/15 00:43:01 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <messyformat.hpp>

#include <cstdio>
#include <stdexcept>
#include <vector>

namespace ftpp {

std::string messyformat(char const *fmt, ...) {
  std::string result;
  va_list args;
  va_start(args, fmt);
  vmessyformat(fmt, args).swap(result);
  va_end(args);
  return result;
}

// The utter shit is that va_copy is a C99 feature and was not included in
// C++98. Obviously C99 would be newer.

std::string vmessyformat(char const *fmt, va_list args) {
  std::vector<char> buffer(MESSYFORMAT_BUFFERSIZE);
  int res = std::vsnprintf(buffer.data(), buffer.size(), fmt, args);
  if (res < 0 || buffer.size() < static_cast<std::size_t>(res))
    throw std::runtime_error("vsnprintf");
  buffer.resize(res);
  return std::string(buffer.data(), buffer.size());
}

} // namespace ftpp
