/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Json.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 22:21:27 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/31 15:18:29 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Json.hpp>

#include <Any.hpp>

template ftjson::Object &ftpp::Any::as<ftjson::Object>();
template ftjson::Array &ftpp::Any::as<ftjson::Array>();
template ftjson::String &ftpp::Any::as<ftjson::String>();
template ftjson::Number &ftpp::Any::as<ftjson::Number>();
template ftjson::Boolean &ftpp::Any::as<ftjson::Boolean>();

template ftjson::Object const &ftpp::Any::as<ftjson::Object>() const;
template ftjson::Array const &ftpp::Any::as<ftjson::Array>() const;
template ftjson::String const &ftpp::Any::as<ftjson::String>() const;
template ftjson::Number const &ftpp::Any::as<ftjson::Number>() const;
template ftjson::Boolean const &ftpp::Any::as<ftjson::Boolean>() const;
