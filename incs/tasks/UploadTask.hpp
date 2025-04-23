/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadTask.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:00:43 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 02:45:04 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "locations/LocationUpload.hpp"
#include "tasks/Task.hpp"

class UploadTask : public Task {
private:
  LocationUpload const &_location;

public:
  UploadTask(Connection::Cycle &cycle, LocationUpload const &location);
  ~UploadTask();

  void onData(std::vector<char> const &data);
  void onEof();
  void onCancel();
};
