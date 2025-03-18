/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surrogate.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 04:55:32 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/24 04:57:53 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace ftpp {

bool ishghtsurrogate(unsigned int code);
bool islowsurrogate(unsigned int code);
unsigned int surrogatepair(unsigned int high, unsigned int low);

} // namespace ftpp
