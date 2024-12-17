/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonValue.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:21:27 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/18 03:48:11 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <JsonValue.hpp>

namespace ftjson {

JsonValue::JsonValue() {
}

JsonValue::JsonValue(JsonValue const &rhs) {
  (void)rhs;
}

JsonValue &JsonValue::operator=(JsonValue const &rhs) {
  (void)rhs;
  return *this;
}

JsonValue::~JsonValue() {
}

} // namespace ftjson
