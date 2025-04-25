/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationDetail.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 02:52:27 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/25 22:25:25 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Connection.hpp"
#include "configs/Location.hpp"
#include "tasks/Task.hpp"

#include <ftjson/Json.hpp>

class Location::Detail {
public:
  typedef Location::Detail *(*Factory)(ftjson::Object const &location);
  typedef std::map<std::string, Factory> Factories;
  static Factories factories;

protected:
  Detail();
  Detail(Detail const &rhs);
  Detail(ftjson::Object const &location);
  Detail &operator=(Detail const &rhs);

public:
  virtual ~Detail();
  virtual Detail *clone() const = 0;
  virtual Task *createTask(Connection::Cycle &cycle) const = 0;
};
