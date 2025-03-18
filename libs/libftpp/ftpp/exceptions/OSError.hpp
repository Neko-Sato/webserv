/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OSError.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:37:23 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/25 04:25:23 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cerrno>
#include <exception>
#include <string>

namespace ftpp {

class OSError : public std::exception {
private:
  int _errno;
  std::string _s;

public:
  OSError(int __errno = 0, std::string const &s = "OSError");
  OSError(OSError const &rhs);
  ~OSError() throw();
  OSError &operator=(OSError const &rhs);

  int get_errno() const;
  char const *what() const throw();
};

} // namespace ftpp
