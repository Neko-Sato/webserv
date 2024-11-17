/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSelector.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:35:30 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 18:34:59 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include <exceptions/OSError.hpp>
#include <selectors/BaseSelector.hpp>

namespace ftpp {

BaseSelector::BaseSelector() {
}

BaseSelector::~BaseSelector() {
}

const char *BaseSelector::RegisteredError::what() const throw() {
  return "File descriptor already registered for selector.";
}

const char *BaseSelector::NotRegisteredError::what() const throw() {
  return "File descriptor not registered in selector.";
}

} // namespace ftpp
