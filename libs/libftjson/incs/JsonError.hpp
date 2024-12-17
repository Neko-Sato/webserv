/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonError.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:15:48 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/18 03:47:19 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception>
#include <string>

namespace ftjson {

class JsonError : std::exception {
private:
  std::string _msg;

public:
  JsonError(std::string const &msg = "unkown error");
  JsonError(JsonError const &rhs);
  ~JsonError() throw();
  JsonError &operator=(JsonError const &rhs);

  char const *what() throw();
};

} // namespace ftjson
