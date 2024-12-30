/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationDefault.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:21:52 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/30 18:42:04 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "BaseLocation.hpp"

#include <Any.hpp>

#include <map>
#include <string>
#include <vector>

class LocationDefault : public BaseLocation {
public:
  struct Cgi {
    std::string path;
    std::vector<std::string> args;
  };

private:
  std::string _root;
  std::vector<std::string> _index;
  bool _autoindex;
  std::map<std::string, Cgi> _cgi;

  static std::string _takeRoot(ftpp::Any const &value);
  static std::vector<std::string> _takeIndex(ftpp::Any const &value);
  static bool _takeAutoindex(ftpp::Any const &value);
  static std::map<std::string, Cgi> _takeCgi(ftpp::Any const &value);

  LocationDefault();

public:
  LocationDefault(ftpp::Any const &value);
  LocationDefault(LocationDefault const &rhs);
  LocationDefault &operator=(LocationDefault const &rhs);
  ~LocationDefault();

  LocationDefault *copy() const;
};
