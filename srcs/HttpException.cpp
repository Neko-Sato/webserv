/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpException.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:43:16 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 02:20:54 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpException.hpp"

HttpException::HttpException(int code) : _code(code) {
}

HttpException::HttpException(HttpException const &rhs)
    : exception(rhs), _code(rhs._code) {
}

HttpException &HttpException::operator=(HttpException const &rhs) {
  if (this != &rhs)
    _code = rhs._code;
  return *this;
}

HttpException::~HttpException() throw() {
}

int HttpException::getCode() const throw() {
  return _code;
}

const char *HttpException::what() const throw() {
  return "HTTP Exception";
}
