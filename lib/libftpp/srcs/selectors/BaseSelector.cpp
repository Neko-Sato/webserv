/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSelector.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:35:30 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/21 17:25:45 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include <exceptions/OSError.hpp>
#include <selectors/BaseSelector.hpp>

namespace ftpp {

unsigned int const BaseSelector::READ = 1;
unsigned int const BaseSelector::WRITE = 2;
unsigned int const BaseSelector::EXCEPT = 4;

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
