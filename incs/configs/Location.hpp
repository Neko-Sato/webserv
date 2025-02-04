/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:21:38 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/04 21:58:27 by hshimizu         ###   ########.fr       */
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
  public:
    typedef Detail *(*factory)(ftjson::Object const &location);
    typedef std::map<std::string, factory> Factories;
    static Factories factories;

  protected:
    Detail();
    Detail(ftjson::Object const &location);
    Detail &operator=(Detail const &rhs);

  public:
    virtual ~Detail();
    virtual Detail *clone() const = 0;
  };

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

  std::string const &getPath() const;
  AllowMethods const &getAllowMethods() const;
  Detail const &getDetail() const;

  bool match(std::string const &method, std::string const &path) const;
};
