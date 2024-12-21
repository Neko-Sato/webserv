/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonObject.hpp                                     :+:      :+:    :+:   */
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

#include <map>

namespace ftjson {

class JsonObject : public JsonValue {
public:
  JsonObject();
  JsonObject(JsonObject const &rhs);
  ~JsonObject();
  JsonObject &operator=(JsonObject const &rhs);

  typedef std::map<std::string, Json> Values;
  Values value;

  type getType() const;
  JsonValue *copy() const;
};

} // namespace ftjson
