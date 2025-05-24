/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ValidationError.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:43:16 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/25 04:24:13 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <string>

class ValidationError : public std::exception {
private:
  std::string _message;

public:
  ValidationError(std::string const &message);
  ValidationError(ValidationError const &rhs);
  ~ValidationError() throw();
  ValidationError &operator=(ValidationError const &rhs);

  char const *what() const throw();
};
