/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationDefault.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:38:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/06/28 03:53:26 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/LocationDetail.hpp"
#include "tasks/Task.hpp"

class LocationDefault : public Location::Detail {
public:
  struct Cgi {
    std::string bin;

    void swap(Cgi &rhs) throw();
  };

  typedef std::set<std::string> Indexes;
  typedef std::map<std::string, Cgi> Cgis;

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
  Cgis const &getCgis() const;

  Task *createTask(Connection::Cycle &cycle) const;
};
