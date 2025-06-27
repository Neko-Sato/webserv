/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationRedirect.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:38:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 03:25:09 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/LocationDetail.hpp"
#include "tasks/Task.hpp"

class LocationRedirect : public Location::Detail {
private:
  int _code;
  std::string _redirect;

  void _takeCode(ftjson::Object const &location);
  void _takeRedirect(ftjson::Object const &location);

public:
  LocationRedirect();
  LocationRedirect(ftjson::Object const &location);
  LocationRedirect(LocationRedirect const &rhs);
  LocationRedirect &operator=(LocationRedirect const &rhs);
  ~LocationRedirect();
  void swap(LocationRedirect &rhs) throw();
  LocationRedirect *clone() const;

  int getCode() const;
  std::string const &getRedirect() const;

  Task *createTask(Connection::Cycle &cycle, std::string const &path) const;
};
