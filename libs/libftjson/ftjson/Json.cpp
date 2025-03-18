/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Json.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:21:27 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:55:51 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftjson/Json.hpp>

#include <ftpp/any/Any.hpp>

// So this file is useless. It's just a bunch of template instantiations.

#define FTPP_ANY_INSTANTIATE_AS(T)                                             \
  template ftpp::Any::Any(T const &value);                                     \
  template ftpp::Any &ftpp::Any::operator=(T const &rhs);                      \
  template T &ftpp::Any::as<T>();                                              \
  template T const &ftpp::Any::as<T>() const;                                  \
  template T &ftpp::Any::as_unsafe<T>();                                       \
  template T const &ftpp::Any::as_unsafe<T>() const;                           \
  template bool ftpp::Any::isType<T>() const;

FTPP_ANY_INSTANTIATE_AS(ftjson::Object)
FTPP_ANY_INSTANTIATE_AS(ftjson::Array)
FTPP_ANY_INSTANTIATE_AS(ftjson::String)
FTPP_ANY_INSTANTIATE_AS(ftjson::Number)
FTPP_ANY_INSTANTIATE_AS(ftjson::Boolean)

#undef FTPP_ANY_INSTANTIATE_AS