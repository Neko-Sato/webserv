/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messyformat.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 20:55:26 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/14 23:39:11 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdarg>
#include <string>

#if !defined(MESSYFORMAT_BUFFERSIZE)
#define MESSYFORMAT_BUFFERSIZE 1024
#endif

namespace ftpp {

std::string messyformat(char const *fmt, ...);
std::string vmessyformat(char const *fmt, va_list args);

} // namespace ftpp
