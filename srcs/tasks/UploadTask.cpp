/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadTask.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:00:43 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/17 20:49:32 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/UploadTask.hpp"

UploadTask::UploadTask(ftev::StreamConnectionTransport &transport,
                       ftev::EventLoop::DeferWatcher &complete,
                       Request const &request, LocationUpload const &location)
    : Task(transport, complete, request), _location(location) {
  (void)_location;
}

UploadTask::~UploadTask() {
}

void UploadTask::onData(std::vector<char> const &) {
}

void UploadTask::onEof() {
  ftev::StreamConnectionTransport &transport = getTransport();
  transport.write("HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello World!\n",
                  52);
  complete();
}
