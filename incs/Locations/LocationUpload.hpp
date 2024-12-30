/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationUpload.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:21:52 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/30 18:46:51 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "BaseLocation.hpp"

#include <Any.hpp>

#include <string>

class LocationUpload : public BaseLocation {
private:
  std::string _store;

  static std::string _takeStore(ftpp::Any const &value);

  LocationUpload();

public:
  LocationUpload(ftpp::Any const &value);
  LocationUpload(LocationUpload const &rhs);
  LocationUpload &operator=(LocationUpload const &rhs);
  ~LocationUpload();

  LocationUpload *copy() const;
};
