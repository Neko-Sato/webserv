/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OSError.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:37:23 by hshimizu          #+#    #+#             */
/*   Updated: 2024/10/13 17:52:50 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exceptions/OSError.hpp"

#include <cstring>

OSError::OSError(int __errno) : _errno(__errno) {
}

char const *OSError::what() const throw() {
  return std::strerror(_errno);
}
