/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseLocation.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:21:38 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/30 18:39:36 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Any.hpp>

#include <set>
#include <string>

class BaseLocation {
public:
  typedef std::set<std::string> AllowMethods;

private:
  AllowMethods _allow_methods;

  static AllowMethods _takeAllowMethods(ftpp::Any const &value);

protected:
  BaseLocation();
  BaseLocation(ftpp::Any const &value);
  BaseLocation(BaseLocation const &rhs);
  virtual BaseLocation &operator=(BaseLocation const &rhs);

public:
  static BaseLocation *create(ftpp::Any const &value);
  virtual ~BaseLocation();
  virtual BaseLocation *copy() const = 0;

  AllowMethods const &getAllowMethods() const;
};
