/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Json.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:21:27 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/17 23:47:44 by hshimizu         ###   ########.fr       */
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

// C++98 does not have extern template.
// Templates are instantiated for each object file,
// which is very inefficient.
// This can be resolved through explicit instantiation and declaration, 
// but this is not possible in C++98.
// This feature was introduced in C++11.
// I am leaving this note here to acknowledge that fact.
// #define FTPP_ANY_DECLARATION_AS(T)                                             \
//   extern template ftpp::Any::Any(T const &value);                              \
//   extern template ftpp::Any &ftpp::Any::operator=(T const &rhs);               \
//   extern template T &ftpp::Any::as<T>();                                       \
//   extern template T const &ftpp::Any::as<T>() const;                           \
//   extern template T &ftpp::Any::as_unsafe<T>();                                \
//   extern template T const &ftpp::Any::as_unsafe<T>() const;                    \
//   extern template bool ftpp::Any::isType<T>() const;

// FTPP_ANY_DECLARATION_AS(ftjson::Object)
// FTPP_ANY_DECLARATION_AS(ftjson::Array)
// FTPP_ANY_DECLARATION_AS(ftjson::String)
// FTPP_ANY_DECLARATION_AS(ftjson::Number)
// FTPP_ANY_DECLARATION_AS(ftjson::Boolean)

// #undef FTPP_ANY_DECLARATION_AS
