/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   App.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 00:49:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/27 00:58:48 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "Cycle.hpp"
#include "configs/LocationDetail.hpp"

App::App(Connection::Cycle &cycle) : cycle(cycle), _task(NULL), _bodySize(0) {
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
  _bodySize += data.size();
  if (_task && _bodySize > cycle.getServerConf().getClientMaxBodySize()) {
    _task->onCancel();
    delete _task;
    _task = NULL;
  }
  if (_task)
    _task->onData(data);
}

void App::onEof() {
  if (_task)
    _task->onEof();
  else if (_bodySize > cycle.getServerConf().getClientMaxBodySize())
    cycle.sendErrorPage(413);
  else
    cycle.sendErrorPage(404);
}
