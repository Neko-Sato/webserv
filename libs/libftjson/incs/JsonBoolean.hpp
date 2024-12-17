/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonBoolean.hpp                                    :+:      :+:    :+:   */
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

class JsonBoolean : public JsonValue {
public:
  JsonBoolean(bool value = false);
  JsonBoolean(JsonBoolean const &rhs);
  ~JsonBoolean();
  JsonBoolean &operator=(JsonBoolean const &rhs);

  bool value;

  type getType() const;
  JsonValue *copy() const;
};

} // namespace ftjson
