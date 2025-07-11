/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   App.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 00:49:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/06/28 03:52:50 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "Cycle.hpp"
#include "configs/LocationDetail.hpp"

App::App(Connection::Cycle &cycle)
    : cycle(cycle), _status(-1), _task(NULL), _bodySize(0) {
  ServerConf const &serverConf = cycle.getServerConf();
  Request const &request = cycle.getRequest();
  ServerConf::Locations::const_iterator location = serverConf.findLocation(request.uri.getPath());
  if (location == serverConf.getLocations().end())
    _status = 404;
  else {
    Location::AllowMethods const &allowMethods = location->second.getAllowMethods();
    if (!allowMethods.empty() &&
        std::find(allowMethods.begin(), allowMethods.end(), request.method) ==
            allowMethods.end())
      _status = 405;
    else 
      _task = location->second.getDetail().createTask(cycle);
  }
}

App::~App() {
  delete _task;
}

void App::onData(std::vector<char> const &data) {
  _bodySize += data.size();
  if (_task && _bodySize > cycle.getServerConf().getClientMaxBodySize()) {
    delete _task;
    _task = NULL;
    if (_status == -1)
      _status = 413;
  }
  if (_task)
    _task->onData(data);
}

void App::onEof() {
  if (_task)
    _task->onEof();
  else
    cycle.sendErrorPage(_status);
}
