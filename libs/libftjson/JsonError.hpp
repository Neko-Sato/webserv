/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JsonError.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 00:15:48 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/24 02:07:43 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception>
#include <string>

namespace ftjson {

class JsonError : public std::exception {
private:
  std::string _msg;

public:
  JsonError(std::string const &msg = "unkown error");
  JsonError(JsonError const &rhs);
  ~JsonError() throw();
  JsonError &operator=(JsonError const &rhs);

  char const *what() const throw();
};

} // namespace ftjson
