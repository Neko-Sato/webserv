/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationUpload.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:21:52 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 00:31:14 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configs/Locations/LocationUpload.hpp"

#include <stdexcept>

LocationUpload::LocationUpload(ftpp::Any const &value) : Location(value) {
  ftjson::Object const &location = value.as<ftjson::Object>();
  _takeStore(location);
}

LocationUpload::LocationUpload(LocationUpload const &rhs)
    : Location(rhs), _store(rhs._store) {
}

LocationUpload &LocationUpload::operator=(LocationUpload const &rhs) {
  if (this != &rhs) {
    Location::operator=(rhs);
    _store = rhs._store;
  }
  return *this;
}

LocationUpload::~LocationUpload() {
}

LocationUpload *LocationUpload::copy() const {
  return new LocationUpload(*this);
}

void LocationUpload::_takeStore(ftjson::Object const &location) {
  ftjson::Object::const_iterator const &it = location.find("store");
  if (it != location.end())
    _store = it->second.as<ftjson::String>();
  else
    throw std::runtime_error("LocationUpload: store is required");
}
