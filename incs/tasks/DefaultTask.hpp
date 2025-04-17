/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultTask.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:53:55 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/17 20:42:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "locations/LocationDefault.hpp"
#include "tasks/Task.hpp"

class DefaultTask : public ::Task {
private:
  LocationDefault const &_location;

public:
  DefaultTask(ftev::StreamConnectionTransport &transport,
              ftev::EventLoop::DeferWatcher &complete, Request const &request,
              LocationDefault const &location);
  ~DefaultTask();

  void onData(std::vector<char> const &data);
  void onEof();
};
