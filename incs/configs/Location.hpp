/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:21:38 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/23 17:10:06 by hshimizu         ###   ########.fr       */
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

  class Detail {
  protected:
    Detail();
    Detail(ftjson::Object const &location);
    Detail &operator=(Detail const &rhs);

  public:
    virtual ~Detail();
    virtual Detail *copy() const = 0;
  };

  typedef Detail *(*detail_factory)(ftjson::Object const &location);
  typedef std::map<std::string, detail_factory> DetailFactories;
  static DetailFactories detail_factories;

private:
  std::string _path;
  AllowMethods _allow_methods;
  Detail *_detail;

  void _takePath(ftjson::Object const &location);
  void _takeAllowMethods(ftjson::Object const &location);
  void _takeDetail(ftjson::Object const &location);

public:
  Location();
  Location(ftpp::Any const &value);
  Location(Location const &rhs);
  Location &operator=(Location const &rhs);
  ~Location();
  void swap(Location &rhs);
};
