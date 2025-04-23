/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:21:38 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 03:23:42 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftjson/Json.hpp>
#include <ftpp/smart_ptr/ScopedPtr.hpp>

#include <set>
#include <string>

class Location {
public:
  typedef std::set<std::string> AllowMethods;
  class Detail;

private:
  AllowMethods _allowMethods;
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
