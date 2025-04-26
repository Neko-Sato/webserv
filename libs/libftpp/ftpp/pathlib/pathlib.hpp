/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 02:50:21 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/27 03:58:27 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

namespace ftpp {

std::string normpath(std::string const &path);

bool isAbsolutePath(std::string const &path);

} // namespace ftpp
