/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 04:30:35 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/22 09:00:03 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Json.hpp>
#include <JsonLexer.hpp>

#include <stack>

namespace ftjson {

class JsonParser {
private:
  JsonLexer _lexer;

  enum state {
    OBJECT,
    ARRAY,
  };

  std::stack<state> _state;

  JsonParser(std::istream &stream);
  ~JsonParser();

  Json _parse();
  static std::string _string_dequote(std::string const &str);

  JsonParser(JsonParser const &rhs);
  JsonParser &operator=(JsonParser const &rhs);

public:
  static Json parse(char const *str);
  static Json parse(std::istream &stream);
};

} // namespace ftjson
