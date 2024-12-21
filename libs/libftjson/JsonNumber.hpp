/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonNumber.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:11:12 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/19 02:53:00 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <JsonValue.hpp>

namespace ftjson {

class JsonNumber : public JsonValue {
public:
  JsonNumber(double value = .0);
  JsonNumber(JsonNumber const &rhs);
  ~JsonNumber();
  JsonNumber &operator=(JsonNumber const &rhs);

  double value;

  type getType() const;
  JsonValue *copy() const;
};

} // namespace ftjson
