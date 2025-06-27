/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationUpload.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:38:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/24 05:31:23 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/LocationDetail.hpp"
#include "tasks/Task.hpp"

class LocationUpload : public Location::Detail {
private:
  std::string _store;
  std::string _redirect;

  void _takeStore(ftjson::Object const &location);
  void _takeRedirect(ftjson::Object const &location);

public:
  LocationUpload();
  LocationUpload(ftjson::Object const &location);
  LocationUpload(LocationUpload const &rhs);
  LocationUpload &operator=(LocationUpload const &rhs);
  ~LocationUpload();
  void swap(LocationUpload &rhs) throw();
  LocationUpload *clone() const;

  std::string const &getStore() const;
  std::string const &getRedirect() const;

  Task *createTask(Connection::Cycle &cycle, std::string const &path) const;
};
