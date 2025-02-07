/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:21:38 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/08 02:12:33 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Any.hpp>
#include <Json.hpp>
#include <smart_ptr/ScopedPtr.hpp>

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

    static Factories initFactories();
    template <typename T> static Detail *create(ftjson::Object const &location);

  protected:
    Detail();
    Detail(Detail const &rhs);
    Detail(ftjson::Object const &location);
    Detail &operator=(Detail const &rhs);

  public:
    virtual ~Detail();
    virtual Detail *clone() const = 0;
  };

private:
  AllowMethods _allow_methods;
  ftpp::ScopedPtr<Detail> _detail;

  void _takeAllowMethods(ftjson::Object const &location);
  void _takeDetail(ftjson::Object const &location);

public:
  Location();
  Location(ftjson::Object const &location);
  Location(Location const &rhs);
  Location &operator=(Location const &rhs);
  ~Location();
  void swap(Location &rhs) throw();

  AllowMethods const &getAllowMethods() const;
  Detail const &getDetail() const;
};

template <typename T>
Location::Detail *Location::Detail::create(ftjson::Object const &location) {
  return new T(location);
}
