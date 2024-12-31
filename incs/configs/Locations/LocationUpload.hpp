/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationUpload.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:21:52 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 00:22:54 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/Location.hpp"

#include <Any.hpp>
#include <Json.hpp>

#include <string>

class LocationUpload : public Location {
private:
  std::string _store;

  void _takeStore(ftjson::Object const &location);

  LocationUpload();

public:
  LocationUpload(ftpp::Any const &value);
  LocationUpload(LocationUpload const &rhs);
  LocationUpload &operator=(LocationUpload const &rhs);
  ~LocationUpload();

  LocationUpload *copy() const;
};
