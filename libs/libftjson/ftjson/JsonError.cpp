/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonError.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:15:48 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:55:51 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftjson/JsonError.hpp>

namespace ftjson {

JsonError::JsonError(std::string const &msg)
    : _msg("JsonError" + (msg.empty() ? "" : (": " + msg))) {
}

JsonError::JsonError(JsonError const &rhs) : _msg(rhs._msg) {
}

JsonError::~JsonError() throw() {
}

JsonError &JsonError::operator=(JsonError const &rhs) {
  if (this != &rhs) {
    _msg = rhs._msg;
  }
  return *this;
}

char const *JsonError::what() const throw() {
  return _msg.c_str();
}

} // namespace ftjson
