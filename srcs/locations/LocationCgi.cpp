/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationCgi.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:38:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 16:10:53 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "locations/LocationCgi.hpp"
#include "ValidationError.hpp"

#include <ftpp/pathlib/pathlib.hpp>

LocationCgi::LocationCgi() {
}

LocationCgi::LocationCgi(ftjson::Object const &location) : LocationStatic(location) {
  _takeCgis(location);
}

LocationCgi::LocationCgi(LocationCgi const &rhs)
    : LocationStatic(rhs), _cgis(rhs._cgis) {
}

LocationCgi &LocationCgi::operator=(LocationCgi const &rhs) {
  if (this != &rhs)
    LocationCgi(rhs).swap(*this);
  return *this;
}

LocationCgi::~LocationCgi() {
}

void LocationCgi::swap(LocationCgi &rhs) throw() {
  LocationStatic::swap(rhs);
  _cgis.swap(rhs._cgis);
}

LocationCgi *LocationCgi::clone() const {
  return new LocationCgi(*this);
}

void LocationCgi::_takeCgis(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("cgi");
  if (it != location.end()) {
    if (!it->second.isType<ftjson::Array>())
      throw ValidationError("cgi is not array");
    ftjson::Array const &cgis = it->second.as_unsafe<ftjson::Array>();
    for (ftjson::Array::const_iterator it = cgis.begin(); it != cgis.end();
         ++it) {
      if (!it->isType<ftjson::Object>())
        throw ValidationError("cgi is not object");
      ftjson::Object const &cgi = it->as_unsafe<ftjson::Object>();
      ftjson::Object::const_iterator jt = cgi.find("ext");
      if (jt == cgi.end())
        throw ValidationError("cgi without ext");
      if (!jt->second.isType<ftjson::String>())
        throw ValidationError("cgi ext is not string");
      std::string const &ext = jt->second.as_unsafe<ftjson::String>();
      if (ext.empty())
        throw ValidationError("cgi ext is empty");
      if (ext[0] != '.')
        throw ValidationError("cgi ext is not start with dot");
      Cgi tmp;
      jt = cgi.find("bin");
      if (jt == cgi.end())
        throw ValidationError("cgi without bin");
      if (!jt->second.isType<ftjson::String>())
        throw ValidationError("cgi bin is not string");
      std::string const &bin = jt->second.as_unsafe<ftjson::String>();
      if (bin.empty())
        throw ValidationError("cgi bin is empty");
      if (!ftpp::isAbsolutePath(bin))
        throw ValidationError("cgi bin is not absolute path");
      tmp.bin = bin;
      _cgis[ext] = tmp;
    }
  }
}

LocationCgi::Cgis const &LocationCgi::getCgis() const {
  return _cgis;
}
