/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathlib.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 02:50:21 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/16 21:02:10 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

namespace ftpp {

std::string pathjoin(std::string const &path1, std::string const &path2);
std::string normpath(std::string const &path);
std::string getSuffix(const std::string &path);

bool isAbsolutePath(std::string const &path);

} // namespace ftpp
