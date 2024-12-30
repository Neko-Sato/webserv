/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationUpload.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:21:52 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/30 18:47:12 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Locations/LocationUpload.hpp"

#include <stdexcept>

LocationUpload::LocationUpload(ftpp::Any const &value)
    : BaseLocation(value), _store(_takeStore(value)) {
}

LocationUpload::LocationUpload(LocationUpload const &rhs)
    : BaseLocation(rhs), _store(rhs._store) {
}

LocationUpload &LocationUpload::operator=(LocationUpload const &rhs) {
  if (this != &rhs) {
    BaseLocation::operator=(rhs);
    _store = rhs._store;
  }
  return *this;
}

LocationUpload::~LocationUpload() {
}

LocationUpload *LocationUpload::copy() const {
  return new LocationUpload(*this);
}
