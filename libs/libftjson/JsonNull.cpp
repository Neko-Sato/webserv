/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonNull.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:11:12 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/18 04:20:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <JsonNull.hpp>

namespace ftjson {

JsonNull::JsonNull() {
}

JsonNull::JsonNull(JsonNull const &rhs) : JsonValue(rhs) {
}

JsonNull::~JsonNull() {
}

JsonNull &JsonNull::operator=(JsonNull const &rhs) {
  (void)rhs;
  return *this;
}

JsonNull::type JsonNull::getType() const {
  return _NULL;
}

JsonValue *JsonNull::copy() const {
  return new JsonNull(*this);
}

} // namespace ftjson
