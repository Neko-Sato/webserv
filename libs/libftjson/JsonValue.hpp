/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonValue.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:21:27 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/19 02:52:36 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <istream>
#include <ostream>

namespace ftjson {

class JsonValue {
protected:
  JsonValue();
  JsonValue(JsonValue const &rhs);
  virtual JsonValue &operator=(JsonValue const &rhs);

public:
  virtual ~JsonValue();

  enum type {
    OBJECT,
    ARRAY,
    STRING,
    NUMBER,
    BOOLEAN,
    _NULL,
  };

  virtual type getType() const = 0;
  virtual JsonValue *copy() const = 0;
};

} // namespace ftjson
