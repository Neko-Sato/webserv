/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Json.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:21:27 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/31 15:23:40 by hshimizu         ###   ########.fr       */
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

extern template ftjson::Object &ftpp::Any::as<ftjson::Object>();
extern template ftjson::Array &ftpp::Any::as<ftjson::Array>();
extern template ftjson::String &ftpp::Any::as<ftjson::String>();
extern template ftjson::Number &ftpp::Any::as<ftjson::Number>();
extern template ftjson::Boolean &ftpp::Any::as<ftjson::Boolean>();

extern template ftjson::Object const &ftpp::Any::as<ftjson::Object>() const;
extern template ftjson::Array const &ftpp::Any::as<ftjson::Array>() const;
extern template ftjson::String const &ftpp::Any::as<ftjson::String>() const;
extern template ftjson::Number const &ftpp::Any::as<ftjson::Number>() const;
extern template ftjson::Boolean const &ftpp::Any::as<ftjson::Boolean>() const;
