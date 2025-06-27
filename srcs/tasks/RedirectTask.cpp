/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectTask.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:56:36 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/19 21:48:48 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/RedirectTask.hpp"
#include "Cycle.hpp"
#include "constants.hpp"
#include "structs/Response.hpp"

#include <sstream>

RedirectTask::RedirectTask(Connection::Cycle &cycle,
                           LocationRedirect const &location, std::string const &path)
    : Task(cycle, path), _location(location) {
}

RedirectTask::~RedirectTask() {
}

void RedirectTask::onData(std::vector<char> const &) {
}

void RedirectTask::onEof() {
  Headers headers;
  headers["Location"].push_back(_location.getRedirect());
  headers["Content-Length"].push_back("0");
  cycle.send(_location.getCode(), headers);
  cycle.send(NULL, 0, false);
}
