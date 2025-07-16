/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskUpload.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:00:43 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/16 21:41:01 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "locations/LocationUpload.hpp"
#include "tasks/TaskStatic.hpp"

class TaskUpload : public TaskStatic {
public:
  TaskUpload(WebservApp::Context const &ctx);
  ~TaskUpload();

  void execute();

  void doPost();
  void doDelete();
};
