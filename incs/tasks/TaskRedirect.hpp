/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskRedirect.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:56:36 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/14 08:24:58 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "locations/LocationRedirect.hpp"
#include "WebservApp.hpp"

class TaskRedirect : public WebservApp::Task {
public:
  TaskRedirect(WebservApp::Context const &ctx);
  ~TaskRedirect();

  void execute();
};
