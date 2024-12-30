/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseBaseLocation.cpp                                   :+:      :+:    :+:
 */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:58:50 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/30 17:41:31 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Locations/BaseLocation.hpp"
#include "Locations/LocationDefault.hpp"
#include "Locations/LocationRedirect.hpp"
#include "Locations/LocationUpload.hpp"

#include <Json.hpp>
#include <Any.hpp>

#include <stdexcept>

BaseLocation::BaseLocation() {
}

BaseLocation::BaseLocation(ftpp::Any const &value) {
  ftjson::Object const &location = value.as<ftjson::Object>();
  {
    ftjson::Object::const_iterator it = location.find("allow_methods");
    if (it != location.end()) {
      ftjson::Array const &methods = it->second.as<ftjson::Array>();
      for (ftjson::Array::const_iterator jt = methods.begin();
           jt != methods.end(); ++jt)
        _allow_methods.insert(jt->as<ftjson::String>());
    }
  }
}

BaseLocation::BaseLocation(BaseLocation const &rhs)
    : _allow_methods(rhs._allow_methods) {
}

BaseLocation &BaseLocation::operator=(BaseLocation const &rhs) {
  if (this != &rhs) {
    _allow_methods = rhs._allow_methods;
  }
  return *this;
}

BaseLocation *BaseLocation::create(ftpp::Any const &value) {
  ftjson::Object const &location = value.as<ftjson::Object>();
  {
    ftjson::Object::const_iterator it = location.find("type");
    if (it != location.end()) {
      std::string type = it->second.as<ftjson::String>();
      if (type == "default")
        return new LocationDefault(location);
      if (type == "upload")
        return new LocationUpload(location);
      if (type == "redirect")
        return new LocationRedirect(location);
      throw std::runtime_error("Unknown location type");
    }
  }
  return new LocationDefault(location);
}

BaseLocation::~BaseLocation() {
}

BaseLocation::AllowMethods const &BaseLocation::getAllowMethods() const {
  return _allow_methods;
}
