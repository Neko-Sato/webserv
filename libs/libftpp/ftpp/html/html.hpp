/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   html.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 18:19:21 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/11 18:24:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

namespace ftpp {

std::string htmlEscape(std::string const &str, bool quote = true);
std::string htmlUnescape(std::string const &str);

} // namespace ftpp
