/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadTask.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:00:43 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/17 21:09:47 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "locations/LocationUpload.hpp"
#include "tasks/Task.hpp"

class UploadTask : public Task {
private:
  LocationUpload const &_location;

public:
  UploadTask(ftev::StreamConnectionTransport &transport,
             ftev::EventLoop::DeferWatcher &complete, Request const &request,
             LocationUpload const &location);
  ~UploadTask();

  void onData(std::vector<char> const &data);
  void onEof();
  void onCancel();
};
