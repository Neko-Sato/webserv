/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultTask.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:46:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 21:35:59 by hshimizu         ###   ########.fr       */
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
  cycle.sendErrorPage(200);
}

void DefaultTask::onCancel() {
}
