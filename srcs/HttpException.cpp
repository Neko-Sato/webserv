/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpException.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:43:16 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/28 06:35:56 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpException.hpp"
#include "constants.hpp"

HttpException::HttpException(int status) : _status(status) {
}

HttpException::HttpException(HttpException const &rhs)
    : exception(rhs), _status(rhs._status) {
}

HttpException &HttpException::operator=(HttpException const &rhs) {
  if (this != &rhs)
    _status = rhs._status;
  return *this;
}

HttpException::~HttpException() throw() {
}

int HttpException::getStatus() const {
  return _status;
}

const char *HttpException::what() const throw() {
  return getHttpStatusReason(_status).c_str();
}
