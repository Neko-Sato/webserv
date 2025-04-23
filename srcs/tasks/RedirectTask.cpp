/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectTask.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:56:36 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 03:03:34 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/RedirectTask.hpp"
#include "constants.hpp"
#include "structs/Response.hpp"

#include <sstream>

RedirectTask::RedirectTask(Connection::Cycle &cycle,
                           LocationRedirect const &location)
    : Task(cycle), _location(location) {
  (void)_location;
}

RedirectTask::~RedirectTask() {
}

void RedirectTask::onData(std::vector<char> const &) {
}

void RedirectTask::onEof() {
  //   std::string response;
  //   Response tmp;
  //   tmp.version = "HTTP/1.1";
  //   tmp.status = _location.getCode();
  //   HttpStatusMap::const_iterator it = httpStatusMap.find(tmp.status);
  //   tmp.reason = it != httpStatusMap.end() ? it->second : "Unknown";
  //   tmp.headers["Content-Type"].push_back("text/html");
  //   tmp.headers["Location"].push_back(_location.getRedirect());
  //   composeResponse(response, tmp);
  //   transport.write(response.c_str(), response.size());
}

void RedirectTask::onCancel() {
}
