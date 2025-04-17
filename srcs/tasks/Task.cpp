/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:44:30 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/17 20:42:17 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tasks/Task.hpp"

Task::Task(ftev::StreamConnectionTransport &transport,
           ftev::EventLoop::DeferWatcher &complete, Request const &request)
    : _transport(transport), _complete(complete), _request(request) {
}

Task::~Task() {
}

void Task::complete() {
  _complete.start();
}

ftev::StreamConnectionTransport &Task::getTransport() const {
  return _transport;
}

Request const &Task::getRequest() const {
  return _request;
}
