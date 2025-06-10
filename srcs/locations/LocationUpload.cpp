/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationUpload.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:38:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/06/10 23:44:16 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "locations/LocationUpload.hpp"
#include "ValidationError.hpp"
#include "tasks/UploadTask.hpp"

#include <ftpp/pathlib/pathlib.hpp>

#include <stdexcept>

LocationUpload::LocationUpload() {
}

LocationUpload::LocationUpload(ftjson::Object const &location) {
  _takeStore(location);
  _takeRedirect(location);
}

LocationUpload::LocationUpload(LocationUpload const &rhs)
    : Detail(rhs), _store(rhs._store), _redirect(rhs._redirect) {
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
  _redirect.swap(rhs._redirect);
}

LocationUpload *LocationUpload::clone() const {
  return new LocationUpload(*this);
}

void LocationUpload::_takeStore(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("store");
  if (it == location.end())
    throw ValidationError("store is not found");
  if (!it->second.isType<ftjson::String>())
    throw ValidationError("store is not string");
  std::string const &store = it->second.as_unsafe<ftjson::String>();
  if (store.empty())
    throw ValidationError("store is empty");
  if (!ftpp::isAbsolutePath(store))
	throw ValidationError("store is not absolute path");
  _store = store;
}

void LocationUpload::_takeRedirect(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("redirect");
  if (it == location.end())
    throw ValidationError("redirect is not found");
  if (!it->second.isType<ftjson::String>())
    throw ValidationError("redirect is not string");
  std::string const &redirect = it->second.as_unsafe<ftjson::String>();
  if (redirect.empty())
    throw ValidationError("redirect is empty");
  _redirect = redirect;
}

std::string const &LocationUpload::getStore() const {
  return _store;
}

std::string const &LocationUpload::getRedirect() const {
  return _redirect;
}

Task *LocationUpload::createTask(Connection::Cycle &cycle) const {
  return new UploadTask(cycle, *this);
}
