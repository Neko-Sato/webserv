/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   App.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 00:49:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/28 06:17:15 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "Cycle.hpp"
#include "configs/LocationDetail.hpp"

App::App(Connection::Cycle &cycle)
    : cycle(cycle), _state(-1), _task(NULL), _bodySize(0) {
  ServerConf const &serverConf = cycle.getServerConf();
  Request const &request = cycle.getRequest();
  Location const *location = serverConf.findLocation(request.path);
  if (location) {
    Location::AllowMethods const &allowMethods = location->getAllowMethods();
    if (allowMethods.empty() ||
        std::find(allowMethods.begin(), allowMethods.end(), request.method) !=
            allowMethods.end())
      _task = location->getDetail().createTask(cycle);
    else
      _state = 405;
  } else
    _state = 404;
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
    if (_state == -1)
      _state = 413;
  }
  if (_task)
    _task->onData(data);
}

void App::onEof() {
  if (_task)
    _task->onEof();
  else
    cycle.sendErrorPage(_state);
}
