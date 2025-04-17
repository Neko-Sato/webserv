/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectTask.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:56:36 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/17 21:09:15 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/RedirectTask.hpp"
#include "constants.hpp"

#include <sstream>

RedirectTask::RedirectTask(ftev::StreamConnectionTransport &transport,
                           ftev::EventLoop::DeferWatcher &complete,
                           Request const &request,
                           LocationRedirect const &location)
    : Task(transport, complete, request), _location(location) {
}

RedirectTask::~RedirectTask() {
}

void RedirectTask::onData(std::vector<char> const &) {
}

void RedirectTask::onEof() {
  std::ostringstream oss;
  oss << "HTTP/1.1 " << _location.getCode() << " Moved Permanently" << CRLF;
  oss << "Location: " << _location.getRedirect() << CRLF;
  oss << CRLF;
  std::string const &response = oss.str();
  ftev::StreamConnectionTransport &transport = getTransport();
  transport.write(response.c_str(), response.size());
  complete();
}

void RedirectTask::onCancel() {
}
