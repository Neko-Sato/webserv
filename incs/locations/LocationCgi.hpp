/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationCgi.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:39:02 by uakizuki          #+#    #+#             */
/*   Updated: 2025/07/12 15:52:30 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "locations/LocationStatic.hpp"

class LocationCgi : public LocationStatic {
public:
  struct Cgi {
    std::string bin;
  };

  typedef std::map<std::string, Cgi> Cgis;

private:
  Cgis _cgis;

  void _takeCgis(ftjson::Object const &location);

public:
  LocationCgi();
  LocationCgi(ftjson::Object const &location);
  LocationCgi(LocationCgi const &rhs);
  LocationCgi &operator=(LocationCgi const &rhs);
  ~LocationCgi();
  void swap(LocationCgi &rhs) throw();
  LocationCgi *clone() const;

  Cgis const &getCgis() const;
};
