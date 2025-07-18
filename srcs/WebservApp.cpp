/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservApp.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 00:49:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/18 11:48:05 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebservApp.hpp"
#include "configs/Location.hpp"
#include "tasks/TaskCgi.hpp"
#include "tasks/TaskRedirect.hpp"
#include "tasks/TaskStatic.hpp"
#include "tasks/TaskUpload.hpp"

WebservApp::Context::Context(Connection::Cycle &c, std::vector<char> const &d,
                             std::string const &p, Location const &l)
    : cycle(c), data(d), path(p), location(l) {
}

template <typename T>
static WebservApp::Task *_createTask(WebservApp::Context const &ctx) {
  return new T(ctx);
}

static WebservApp::Task::Factories _initFactories() {
  typedef WebservApp::Task::Factories Factories;
  Factories factories;
  factories[&typeid(LocationStatic)] = _createTask<TaskStatic>;
  factories[&typeid(LocationCgi)] = _createTask<TaskCgi>;
  factories[&typeid(LocationUpload)] = _createTask<TaskUpload>;
  factories[&typeid(LocationRedirect)] = _createTask<TaskRedirect>;
  return factories;
}

WebservApp::Task::Factories WebservApp::Task::factories = _initFactories();

WebservApp::Task *WebservApp::Task::create(Context const &ctx) {
  Task::Factories::const_iterator it =
      Task::factories.find(&typeid(ctx.location));
  if (it == Task::factories.end())
    throw std::runtime_error("Unknown task type");
  return it->second(ctx);
}

WebservApp::Task::Task(WebservApp::Context const &ctx) : ctx(ctx) {
}

WebservApp::Task::~Task() {
}

WebservApp::WebservApp(Connection::Cycle &cycle)
    : App(cycle), _status(-1), _maxBodySize(0), _task(NULL) {
  ServerConf const &serverConf = cycle.getServerConf();
  Request const &request = cycle.getRequest();
  _maxBodySize = serverConf.getClientMaxBodySize();
  _location = serverConf.findLocation(request.uri.getPath());
  if (_location == serverConf.getLocations().end())
    _status = 404;
  else {
    request.uri.getPath().substr(_location->first.size()).swap(_path);
    {
      ftpp::Optional<std::size_t> const &maxBodySize =
          _location->second->getClientMaxBodySize();
      if (maxBodySize)
        _maxBodySize = *maxBodySize;
    }
    Location::AllowMethods const &allowMethods =
        _location->second->getAllowMethods();
    if (!allowMethods.empty() &&
        std::find(allowMethods.begin(), allowMethods.end(), request.method) ==
            allowMethods.end())
      _status = 405;
  }
}

WebservApp::~WebservApp() {
  delete _task;
}

void WebservApp::onData(std::vector<char> const &data) {
  if (_status != 413) {
    _buffer.insert(_buffer.end(), data.begin(), data.end());
    if (_buffer.size() > _maxBodySize) {
      _status = 413;
      _buffer.clear();
    }
  }
}

void WebservApp::onEof() {
  if (_status == -1) {
    _task = Task::create(Context(cycle, _buffer, _path, *_location->second));
    _task->execute();
  } else
    cycle.sendErrorPage(_status);
}
