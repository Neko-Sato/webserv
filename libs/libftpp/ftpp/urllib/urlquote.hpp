/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   urlquote.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 20:20:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:28:06 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

namespace ftpp {

std::string urlquote(std::string const &s);
std::string urlunquote(std::string const &s);

}; // namespace ftpp
