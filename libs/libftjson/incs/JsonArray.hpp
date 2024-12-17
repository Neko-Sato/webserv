/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonArray.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:11:12 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/18 04:20:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Json.hpp>
#include <JsonValue.hpp>

#include <vector>

namespace ftjson {

class JsonArray : public JsonValue {
public:
  JsonArray();
  JsonArray(JsonArray const &rhs);
  ~JsonArray();
  JsonArray &operator=(JsonArray const &rhs);

  std::vector<Json> value;

  type getType() const;
  JsonValue *copy() const;
};

} // namespace ftjson
