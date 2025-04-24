/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   App.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 00:49:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 21:35:16 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "Cycle.hpp"
#include "configs/LocationDetail.hpp"

App::App(Connection::Cycle &cycle) : cycle(cycle), _task(NULL) {
  ServerConf const &serverConf = cycle.getServerConf();
  Request const &request = cycle.getRequest();
  Location const *location =
      serverConf.findLocation(request.method, request.path);
  if (location)
    _task = location->getDetail().createTask(cycle);
}

App::~App() {
  delete _task;
}

void App::onData(std::vector<char> const &data) {
  if (_task)
    _task->onData(data);
}

void App::onEof() {
  if (_task)
    _task->onEof();
  else
    cycle.sendErrorPage(404);
}

void App::onCancel() {
  if (_task)
    _task->onCancel();
}
