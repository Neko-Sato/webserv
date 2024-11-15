/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSelector copy.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:35:30 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/15 22:47:04 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include <exceptions/OSError.hpp>
#include <selectors/BaseSelector.hpp>

namespace ftpp {

BaseSelector::BaseSelector() {
}

BaseSelector::BaseSelector(BaseSelector const &rhs) {
  (void)rhs;
  assert(false);
}

BaseSelector &BaseSelector::operator=(BaseSelector const &rhs) {
  (void)rhs;
  assert(false);
  return *this;
}

} // namespace ftpp
