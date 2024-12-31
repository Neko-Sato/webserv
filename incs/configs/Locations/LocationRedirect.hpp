/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationRedirect.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:21:52 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 00:22:24 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/Location.hpp"

#include <Any.hpp>
#include <Json.hpp>

#include <string>

class LocationRedirect : public Location {
private:
  int _code;
  std::string _redirect;

  void _takeCode(ftjson::Object const &location);
  void _takeRedirect(ftjson::Object const &location);

  LocationRedirect();

public:
  LocationRedirect(ftpp::Any const &value);
  LocationRedirect(LocationRedirect const &rhs);
  LocationRedirect &operator=(LocationRedirect const &rhs);
  ~LocationRedirect();

  LocationRedirect *copy() const;
};
