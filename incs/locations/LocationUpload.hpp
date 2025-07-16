/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationUpload.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:38:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 15:52:38 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "locations/LocationStatic.hpp"

class LocationUpload : public LocationStatic {
public:
  LocationUpload();
  LocationUpload(ftjson::Object const &location);
  LocationUpload(LocationUpload const &rhs);
  LocationUpload &operator=(LocationUpload const &rhs);
  ~LocationUpload();
  void swap(LocationUpload &rhs) throw();
  LocationUpload *clone() const;
};
