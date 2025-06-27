/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadTask.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:00:43 by hshimizu          #+#    #+#             */
/*   Updated: 2025/06/28 03:54:13 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "locations/LocationUpload.hpp"
#include "tasks/Task.hpp"

#include <deque>

class UploadTask : public Task {
private:
  LocationUpload const &_location;
  int _status;
  std::deque<char> _buffer;
  std::string _boundary;

public:
  UploadTask(Connection::Cycle &cycle, LocationUpload const &location);
  ~UploadTask();

  void onData(std::vector<char> const &data);
  void onEof();
};
