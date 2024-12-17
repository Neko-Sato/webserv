/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonInteger.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:11:12 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/18 04:20:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <JsonValue.hpp>

namespace ftjson {

class JsonInteger : public JsonValue {
public:
  JsonInteger(long value = 0);
  JsonInteger(JsonInteger const &rhs);
  ~JsonInteger();
  JsonInteger &operator=(JsonInteger const &rhs);

  long value;

  type getType() const;
  JsonValue *copy() const;
};

} // namespace ftjson
