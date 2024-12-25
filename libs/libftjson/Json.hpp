/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Json.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:21:27 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/26 07:18:24 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Variant.hpp>

#include <map>
#include <string>
#include <vector>

namespace ftjson {

typedef std::map<std::string, ftpp::Variant> Object;
typedef std::vector<ftpp::Variant> Array;
typedef std::string String;
typedef double Number;
typedef bool Boolean;
struct Null {};

} // namespace ftjson
