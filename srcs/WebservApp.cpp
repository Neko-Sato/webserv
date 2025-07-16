/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservApp.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 00:49:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/16 22:59:50 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebservApp.hpp"
#include "tasks/TaskStatic.hpp"
#include "tasks/TaskCgi.hpp"
#include "tasks/TaskUpload.hpp"
#include "tasks/TaskRedirect.hpp"
#include "configs/Location.hpp"

WebservApp::Context::Context(Connection::Cycle &c, std::string const &p,
                             std::vector<char> const &d, Location const &l)
    : cycle(c), path(p), data(d), location(l) {}
    
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
  Task::Factories::const_iterator it = Task::factories.find(&typeid(ctx.location));
  if (it == Task::factories.end())
    throw std::runtime_error("Unknown task type");
  return it->second(ctx);
}

WebservApp::Task::Task(WebservApp::Context const &ctx) : ctx(ctx) {
}

WebservApp::Task::~Task() {
}

WebservApp::WebservApp(Connection::Cycle &cycle)
    : App(cycle), _status(-1), _maxBodySize(0), _path(NULL), _location(NULL), _task(NULL) {
  ServerConf const &serverConf = cycle.getServerConf();
  Request const &request = cycle.getRequest();
  ServerConf::Locations::const_iterator location = serverConf.findLocation(request.uri.getPath());
  _maxBodySize = serverConf.getClientMaxBodySize();
  if (location == serverConf.getLocations().end())
    _status = 404;
  else {
    _path = &location->first;
    _location = &*location->second;
    {
      ftpp::Optional<std::size_t> const &maxBodySize = _location->getClientMaxBodySize();
      if (maxBodySize)
        _maxBodySize = *maxBodySize;
    }
    Location::AllowMethods const &allowMethods = _location->getAllowMethods();
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
    _task = Task::create(Context(cycle, *_path, _buffer, *_location));
    _task->execute();
  }
  else
    cycle.sendErrorPage(_status);
}
