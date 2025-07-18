/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strftime.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:08:49 by uakizuki          #+#    #+#             */
/*   Updated: 2025/07/18 13:12:21 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/time/time.hpp>
#include <ftpp/macros.hpp>

#include <ctime>
#include <string>

namespace ftpp {

std::string strftime(char const *format, struct tm const &time) {
  char buffer[256];
  if (unlikely(::strftime(buffer, sizeof(buffer), format, &time) == 0))
    throw std::runtime_error("strftime failed");
  return std::string(buffer);
}

} // namespace ftpp
