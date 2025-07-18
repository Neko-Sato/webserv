/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   literals.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:17:50 by uakizuki          #+#    #+#             */
/*   Updated: 2025/07/18 13:29:36 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/string/string.hpp>

namespace ftpp {

std::string const ASCII_LOWERCASE = "abcdefghijklmnopqrstuvwxyz";
std::string const ASCII_UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
std::string const ASCII_LETTER = ASCII_LOWERCASE + ASCII_UPPERCASE;
std::string const DIGITS = "0123456789";
std::string const ASCII_ALNUM = ASCII_LETTER + DIGITS;

} // namespace ftpp
