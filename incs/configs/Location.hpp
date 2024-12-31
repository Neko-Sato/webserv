/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:21:38 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 00:21:57 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Any.hpp>
#include <Json.hpp>

#include <set>
#include <string>

class Location {
public:
  typedef std::set<std::string> AllowMethods;

private:
  std::string _path;
  AllowMethods _allow_methods;

  void _takePath(ftjson::Object const &location);
  void _takeAllowMethods(ftjson::Object const &location);

protected:
  Location();
  Location(ftpp::Any const &value);
  Location(Location const &rhs);
  virtual Location &operator=(Location const &rhs);

public:
  static Location *create(ftpp::Any const &value);
  virtual ~Location();
  virtual Location *copy() const = 0;

  std::string const &getPath() const;
  AllowMethods const &getAllowMethods() const;
};
