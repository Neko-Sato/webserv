/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationRedirect.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:21:52 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/30 18:42:54 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "BaseLocation.hpp"

#include <Any.hpp>

#include <string>

class LocationRedirect : public BaseLocation {
private:
  int _code;
  std::string _redirect;

  static int _takeCode(ftpp::Any const &value);
  static std::string _takeRedirect(ftpp::Any const &value);

  LocationRedirect();

public:
  LocationRedirect(ftpp::Any const &value);
  LocationRedirect(LocationRedirect const &rhs);
  LocationRedirect &operator=(LocationRedirect const &rhs);
  ~LocationRedirect();

  LocationRedirect *copy() const;
};
