/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPageTask.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:45:11 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/17 20:50:43 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/ErrorPageTask.hpp"

ErrorPageTask::ErrorPageTask(ftev::StreamConnectionTransport &transport,
                             ftev::EventLoop::DeferWatcher &complete,
                             Request const &request,
                             ServerConf::ErrorPages const &errorPages,
                             int statusCode)
    : Task(transport, complete, request), _errorPages(errorPages),
      _statusCode(statusCode) {
  (void)_errorPages;
  (void)_statusCode;
}

ErrorPageTask::~ErrorPageTask() {
}

void ErrorPageTask::onData(std::vector<char> const &) {
}

void ErrorPageTask::onEof() {
  complete();
}
