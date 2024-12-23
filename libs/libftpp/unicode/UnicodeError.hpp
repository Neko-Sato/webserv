/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UnicodeError.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 22:05:06 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/24 00:33:49 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <string>

namespace ftpp {

class UnicodeError : public std::exception {
private:
  std::string _s;

public:
  UnicodeError();
  UnicodeError(std::string const &s);
  UnicodeError(UnicodeError const &rhs);
  ~UnicodeError() throw();
  UnicodeError &operator=(UnicodeError const &rhs);

  char const *what() const throw();
};

} // namespace ftpp
