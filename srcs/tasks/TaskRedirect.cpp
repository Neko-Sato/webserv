/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TaskRedirect.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:56:36 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/16 22:57:53 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/TaskRedirect.hpp"
#include "structs/Response.hpp"
#include "Cycle.hpp"

TaskRedirect::TaskRedirect(WebservApp::Context const &ctx) : Task(ctx) {
}

TaskRedirect::~TaskRedirect() {
}

void TaskRedirect::execute() {
  LocationRedirect const &location = static_cast<LocationRedirect const &>(ctx.location);
  Connection::Cycle &cycle = ctx.cycle;
  Headers headers;
  headers["Location"].push_back(location.getRedirect());
  headers["Content-Length"].push_back("0");
  cycle.send(location.getCode(), headers);
  cycle.send(NULL, 0, false);
}
