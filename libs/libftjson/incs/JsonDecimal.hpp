/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonDecimal.hpp                                    :+:      :+:    :+:   */
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

class JsonDecimal : public JsonValue {
public:
  JsonDecimal(double value = .0);
  JsonDecimal(JsonDecimal const &rhs);
  ~JsonDecimal();
  JsonDecimal &operator=(JsonDecimal const &rhs);

  double value;

  type getType() const;
  JsonValue *copy() const;
};

} // namespace ftjson
