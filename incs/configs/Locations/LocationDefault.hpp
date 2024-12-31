/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationDefault.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:21:52 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 00:22:10 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/Location.hpp"

#include <Any.hpp>

#include <map>
#include <string>
#include <vector>

class LocationDefault : public Location {
public:
  struct Cgi {
    std::string path;
  };

private:
  std::string _root;
  std::vector<std::string> _index;
  bool _autoindex;
  std::map<std::string, Cgi> _cgi;

  void _takeRoot(ftjson::Object const &location);
  void _takeIndex(ftjson::Object const &location);
  void _takeAutoindex(ftjson::Object const &location);
  void _takeCgi(ftjson::Object const &location);

  LocationDefault();

public:
  LocationDefault(ftpp::Any const &value);
  LocationDefault(LocationDefault const &rhs);
  LocationDefault &operator=(LocationDefault const &rhs);
  ~LocationDefault();

  LocationDefault *copy() const;
};
