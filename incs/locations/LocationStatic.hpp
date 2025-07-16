/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationStatic.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:38:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 15:52:35 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/Location.hpp"

#include <vector>

class LocationStatic : public Location {
public:
  typedef std::vector<std::string> IndexFiles;

private:
  std::string _root;
  IndexFiles _index;
  bool _autoindex;

  void _takeRoot(ftjson::Object const &location);
  void _takeIndex(ftjson::Object const &location);
  void _takeAutoindex(ftjson::Object const &location);

public:
  LocationStatic();
  LocationStatic(ftjson::Object const &location);
  LocationStatic(LocationStatic const &rhs);
  LocationStatic &operator=(LocationStatic const &rhs);
  ~LocationStatic();
  void swap(LocationStatic &rhs) throw();
  LocationStatic *clone() const;

  std::string const &getRoot() const;
  IndexFiles const &getIndex() const;
  bool getAutoindex() const;
};
