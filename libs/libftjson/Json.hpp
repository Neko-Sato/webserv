/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Json.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:21:27 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/25 08:23:56 by hshimizu         ###   ########.fr       */
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

  template <typename T> T &as();
  template <typename T> T const &as() const;
};

/*
Basically, there will be no type checking.
This is because it is less likely to ask dynamically and more likely to ask
for a specific type for a specific key. Now, let's try-cath!
*/
template <typename T> T &Json::as() {
  return dynamic_cast<T &>(*_value);
}

template <typename T> T const &Json::as() const {
  return dynamic_cast<T const &>(*_value);
}

} // namespace ftjson
