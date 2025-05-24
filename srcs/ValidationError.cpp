/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ValidationError.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:43:16 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/25 04:25:06 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ValidationError.hpp"

ValidationError::ValidationError(std::string const &message)
	: _message(message) {
}

ValidationError::ValidationError(ValidationError const &rhs)
	: exception(rhs), _message(rhs._message) {
}

ValidationError &ValidationError::operator=(ValidationError const &rhs) {
	if (this != &rhs)
		_message = rhs._message;
	return *this;
}

ValidationError::~ValidationError() throw() {
}
char const *ValidationError::what() const throw() {
	return _message.c_str();
}
