/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Json.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:21:27 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/17 01:56:38 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Any.hpp>

#include <map>
#include <string>
#include <vector>

namespace ftjson {

typedef std::map<std::string, ftpp::Any> Object;
typedef std::vector<ftpp::Any> Array;
typedef std::string String;
typedef double Number;
typedef bool Boolean;

} // namespace ftjson
