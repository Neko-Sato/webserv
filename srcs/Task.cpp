/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Task.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:44:30 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/17 00:45:07 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Task.hpp"

Task::Task(ftev::StreamConnectionTransport &transport,
           ftev::EventLoop::DeferWatcher &complete)
    : _transport(transport), _complete(complete) {
}

Task::~Task() {
}

void Task::complete() {
  _complete.start();
}

ftev::StreamConnectionTransport &Task::getTransport() const {
  return _transport;
}
