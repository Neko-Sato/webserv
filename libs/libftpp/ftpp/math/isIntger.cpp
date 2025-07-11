/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isIntger.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 06:15:48 by uakizuki          #+#    #+#             */
/*   Updated: 2025/07/12 06:17:18 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cmath>

namespace ftpp {

bool isInteger(double value) {
  if (std::isnan(value) || std::isinf(value))
    return false;
  return std::floor(value) == value;
}

} // namespace ftpp
