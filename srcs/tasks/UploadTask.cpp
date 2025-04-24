/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadTask.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:00:43 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 22:38:08 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/UploadTask.hpp"
#include "Cycle.hpp"

UploadTask::UploadTask(Connection::Cycle &cycle, LocationUpload const &location)
    : Task(cycle), _location(location) {
  (void)_location;
}

UploadTask::~UploadTask() {
}

void UploadTask::onData(std::vector<char> const &) {
}

void UploadTask::onEof() {
  Response::Headers headers;
  headers["Content-Type"].push_back("text/plain");
  cycle.send(200, headers);
  cycle.send("UploadTask", 10, false);
}

void UploadTask::onCancel() {
}
