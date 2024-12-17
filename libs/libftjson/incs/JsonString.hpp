/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonString.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:11:12 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/18 04:20:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <JsonValue.hpp>

#include <string>

namespace ftjson {

class JsonString : public JsonValue {
public:
  JsonString(std::string const &value = "");
  JsonString(JsonString const &rhs);
  ~JsonString();
  JsonString &operator=(JsonString const &rhs);

  std::string value;

  type getType() const;
  JsonValue *copy() const;
};

} // namespace ftjson
