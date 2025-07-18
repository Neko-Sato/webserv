/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 13:07:16 by uakizuki          #+#    #+#             */
/*   Updated: 2025/07/18 13:10:57 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ctime>
#include <string>

namespace ftpp {

std::string strftime(char const *format, struct tm const &time);

} // namespace ftpp
