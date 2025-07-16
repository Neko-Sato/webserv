/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:21:38 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 15:51:58 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftjson/Json.hpp>
#include <ftpp/optional/Optional.hpp>

#include <set>
#include <map>
#include <string>


class Location {
public:
  typedef Location *(*Factory)(ftjson::Object const &location);
  typedef std::map<std::string, Factory> Factories;
  static Factories factories;
  static Location *create(ftjson::Object const &location);

  typedef std::set<std::string> AllowMethods;

private:
  AllowMethods _allowMethods;
  ftpp::Optional<std::size_t> _clientMaxBodySize;
  
  void _takeAllowMethods(ftjson::Object const &location);
  void _takeClientMaxBodySize(ftjson::Object const &location);

protected:
  Location();
  Location(ftjson::Object const &location);
  Location(Location const &rhs);
  Location &operator=(Location const &rhs);
  void swap(Location &rhs) throw();

public:
  virtual ~Location();
  virtual Location *clone() const = 0;
  
  AllowMethods const &getAllowMethods() const;
  ftpp::Optional<std::size_t> const &getClientMaxBodySize() const;
};
