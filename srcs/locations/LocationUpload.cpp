/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationUpload.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:38:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 16:11:02 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "locations/LocationUpload.hpp"
#include "ValidationError.hpp"

#include <ftpp/pathlib/pathlib.hpp>

LocationUpload::LocationUpload() {
}

LocationUpload::LocationUpload(ftjson::Object const &location) : LocationStatic(location) {
}

LocationUpload::LocationUpload(LocationUpload const &rhs)
    : LocationStatic(rhs) {
}

LocationUpload &LocationUpload::operator=(LocationUpload const &rhs) {
  if (this != &rhs)
    LocationUpload(rhs).swap(*this);
  return *this;
}

LocationUpload::~LocationUpload() {
}

void LocationUpload::swap(LocationUpload &rhs) throw() {
  LocationStatic::swap(rhs);
}

LocationUpload *LocationUpload::clone() const {
  return new LocationUpload(*this);
}
