/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonLexer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 02:25:31 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/23 19:06:01 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <JsonToken.hpp>

#include <NonCopyable.hpp>

#include <istream>

namespace ftjson {

class JsonLexer : private ftpp::NonCopyable {
private:
  std::istream &_stream;

  JsonLexer();

public:
  JsonLexer(std::istream &stream);
  ~JsonLexer();

  JsonToken nextToken();

private:
  JsonToken _maybe_string();
  JsonToken _maybe_number();
  JsonToken _maybe_keyword();
};

} // namespace ftjson
