/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultTask.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:46:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 22:38:13 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/DefaultTask.hpp"
#include "Cycle.hpp"

DefaultTask::DefaultTask(Connection::Cycle &cycle,
                         LocationDefault const &location)
    : Task(cycle), _location(location) {
  (void)_location;
}

DefaultTask::~DefaultTask() {
}

void DefaultTask::onData(std::vector<char> const &) {
}

void DefaultTask::onEof() {
  Response::Headers headers;
  headers["Content-Type"].push_back("text/plain");
  cycle.send(200, headers);
  cycle.send("DefaultTask", 11, false);
}

void DefaultTask::onCancel() {
}
