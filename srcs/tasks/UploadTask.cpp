/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadTask.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:00:43 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 21:37:00 by hshimizu         ###   ########.fr       */
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
  cycle.sendErrorPage(200);
}

void UploadTask::onCancel() {
}
