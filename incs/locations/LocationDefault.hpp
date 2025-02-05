/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationDefault.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:38:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/05 03:33:50 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <configs/Location.hpp>

class LocationDefault : public Location::Detail {
public:
  typedef std::set<std::string> Indexes;
  typedef std::map<std::string, std::string> Cgis;

private:
  std::string _root;
  Indexes _index;
  bool _autoindex;
  Cgis _cgi;

  void _takeRoot(ftjson::Object const &location);
  void _takeIndex(ftjson::Object const &location);
  void _takeAutoindex(ftjson::Object const &location);
  void _takeCgi(ftjson::Object const &location);

public:
  LocationDefault();
  LocationDefault(ftjson::Object const &location);
  LocationDefault(LocationDefault const &rhs);
  LocationDefault &operator=(LocationDefault const &rhs);
  ~LocationDefault();
  void swap(LocationDefault &rhs) throw();
  LocationDefault *clone() const;

  std::string const &getRoot() const;
  Indexes const &getIndex() const;
  bool getAutoindex() const;
};
