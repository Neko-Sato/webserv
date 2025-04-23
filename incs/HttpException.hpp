/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpException.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:43:16 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 02:20:45 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>

class HttpException : public std::exception {
private:
  int _code;

public:
  HttpException(int code = 500);
  HttpException(HttpException const &rhs);
  ~HttpException() throw();
  HttpException &operator=(HttpException const &rhs);

  char const *what() const throw();

  int getCode() const throw();
};
