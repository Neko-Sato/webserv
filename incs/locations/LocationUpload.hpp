/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationUpload.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:38:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/11 23:20:30 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/Location.hpp"

class LocationUpload : public Location::Detail {
public:
  class Task : public Location::Task {
  private:
    LocationUpload const &_location;

  public:
    Task(ftev::StreamConnectionTransport &transport,
         LocationUpload const &location);
    ~Task();
    void on_data(std::vector<char> const &data);
    void on_eof();
  };

private:
  std::string _store;

  void _takeStore(ftjson::Object const &location);

public:
  LocationUpload();
  LocationUpload(ftjson::Object const &location);
  LocationUpload(LocationUpload const &rhs);
  LocationUpload &operator=(LocationUpload const &rhs);
  ~LocationUpload();
  void swap(LocationUpload &rhs) throw();
  LocationUpload *clone() const;

  std::string const &getStore() const;

  Task *createTask(ftev::StreamConnectionTransport &transport) const;
};
