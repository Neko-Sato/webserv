/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonLexer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 02:25:31 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/24 00:45:05 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <JsonToken.hpp>

#include <istream>

namespace ftjson {

class JsonLexer {
private:
  std::istream &_stream;

  JsonLexer();
  JsonLexer(JsonLexer const &rhs);
  JsonLexer &operator=(JsonLexer const &rhs);

public:
  JsonLexer(std::istream &stream);
  ~JsonLexer();

  JsonToken nextToken();

private:
  void _skip_space();
  JsonToken _maybe_string();
  JsonToken _maybe_number();
};

} // namespace ftjson
