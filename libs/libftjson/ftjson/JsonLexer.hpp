/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonLexer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 02:25:31 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:55:51 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftjson/JsonToken.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>

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
