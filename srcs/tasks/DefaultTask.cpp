/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DefaultTask.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:46:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/17 21:09:09 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/DefaultTask.hpp"

DefaultTask::DefaultTask(ftev::StreamConnectionTransport &transport,
                         ftev::EventLoop::DeferWatcher &complete,
                         Request const &request,
                         LocationDefault const &location)
    : Task(transport, complete, request), _location(location) {
  (void)_location;
}

DefaultTask::~DefaultTask() {
}

void DefaultTask::onData(std::vector<char> const &) {
}

void DefaultTask::onEof() {
  ftev::StreamConnectionTransport &transport = getTransport();
  transport.write("HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello World!\n",
                  52);
  complete();
}

void DefaultTask::onCancel() {
}
