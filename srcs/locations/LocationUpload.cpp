/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationUpload.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:38:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 03:00:31 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <locations/LocationUpload.hpp>
#include <tasks/UploadTask.hpp>

#include <stdexcept>

LocationUpload::LocationUpload() {
}

LocationUpload::LocationUpload(ftjson::Object const &location) {
  _takeStore(location);
}

LocationUpload::LocationUpload(LocationUpload const &rhs)
    : Detail(rhs), _store(rhs._store) {
}

LocationUpload &LocationUpload::operator=(LocationUpload const &rhs) {
  if (this != &rhs)
    LocationUpload(rhs).swap(*this);
  return *this;
}

LocationUpload::~LocationUpload() {
}

void LocationUpload::swap(LocationUpload &rhs) throw() {
  _store.swap(rhs._store);
}

LocationUpload *LocationUpload::clone() const {
  return new LocationUpload(*this);
}

void LocationUpload::_takeStore(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("store");
  if (it == location.end())
    throw std::runtime_error("store is not found");
  if (!it->second.isType<ftjson::String>())
    throw std::runtime_error("store is not string");
  std::string const &store = it->second.as_unsafe<ftjson::String>();
  if (store.empty())
    throw std::runtime_error("store is empty");
  _store = store;
}

std::string const &LocationUpload::getStore() const {
  return _store;
}

Task *LocationUpload::createTask(Connection::Cycle &cycle) const {
  return new UploadTask(cycle, *this);
}
