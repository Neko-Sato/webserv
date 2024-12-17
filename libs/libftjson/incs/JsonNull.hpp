/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonNull.hpp                                       :+:      :+:    :+:   */
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

class JsonNull : public JsonValue {
public:
  JsonNull();
  JsonNull(JsonNull const &rhs);
  ~JsonNull();
  JsonNull &operator=(JsonNull const &rhs);

  type getType() const;
  JsonValue *copy() const;
};

} // namespace ftjson
