/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectTask.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:56:36 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/13 20:24:37 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "locations/LocationRedirect.hpp"
#include "tasks/Task.hpp"

class RedirectTask : public Task {
private:
  LocationRedirect const &_location;

public:
  RedirectTask(Connection::Cycle &cycle, LocationRedirect const &location);
  ~RedirectTask();

  void onData(std::vector<char> const &data);
  void onEof();
};
