/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Json.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:21:27 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/18 04:24:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <JsonNull.hpp>
#include <JsonValue.hpp>

#include <istream>
#include <map>
#include <ostream>

namespace ftjson {

class Json {
private:
  JsonValue *_value;
  Json(JsonValue *value);

public:
  Json(JsonValue const &value = JsonNull());

  Json(Json const &rhs);
  ~Json();
  Json &operator=(Json const &rhs);

  /*
  Basically, there will be no type checking.
  This is because it is less likely to ask dynamically and more likely to ask
  for a specific type for a specific key. Now, let's try-cath!
  */
  JsonValue &getValue();
  JsonValue const &getValue() const;
};

} // namespace ftjson
