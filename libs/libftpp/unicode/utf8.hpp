/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utf8.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 12:25:19 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/24 02:35:15 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <istream>

namespace ftpp {

int utf8_length(int c);
bool utf8_isvalid(char const *buffer, std::size_t size);

std::istream &utf8_getc(std::istream &stream, std::string &result);

} // namespace ftpp
