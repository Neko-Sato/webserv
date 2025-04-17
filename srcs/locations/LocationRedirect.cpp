/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationRedirect.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 18:38:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/17 20:40:35 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "locations/LocationRedirect.hpp"
#include "tasks/RedirectTask.hpp"

#include <stdexcept>

LocationRedirect::LocationRedirect() : _code(0) {
}

LocationRedirect::LocationRedirect(ftjson::Object const &location) {
  _takeCode(location);
  _takeRedirect(location);
}

LocationRedirect::LocationRedirect(LocationRedirect const &rhs)
    : Detail(rhs), _code(rhs._code), _redirect(rhs._redirect) {
}

LocationRedirect &LocationRedirect::operator=(LocationRedirect const &rhs) {
  if (this != &rhs)
    LocationRedirect(rhs).swap(*this);
  return *this;
}

LocationRedirect::~LocationRedirect() {
}

void LocationRedirect::swap(LocationRedirect &rhs) throw() {
  std::swap(_code, rhs._code);
  _redirect.swap(rhs._redirect);
}

LocationRedirect *LocationRedirect::clone() const {
  return new LocationRedirect(*this);
}

void LocationRedirect::_takeCode(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("code");
  if (it == location.end())
    throw std::runtime_error("code is not found");
  if (!it->second.isType<ftjson::Number>())
    throw std::runtime_error("code is not number");
  double code = it->second.as_unsafe<ftjson::Number>();
  if (code < 300 || code >= 400 || code != static_cast<int>(code))
    throw std::runtime_error("code is not valid redirect code");
  _code = static_cast<int>(code);
}

void LocationRedirect::_takeRedirect(ftjson::Object const &location) {
  ftjson::Object::const_iterator it = location.find("redirect");
  if (it == location.end())
    throw std::runtime_error("redirect is not found");
  if (!it->second.isType<ftjson::String>())
    throw std::runtime_error("redirect is not string");
  std::string const &redirect = it->second.as_unsafe<std::string>();
  if (redirect.empty())
    throw std::runtime_error("redirect is empty");
  _redirect = it->second.as_unsafe<std::string>();
}

int LocationRedirect::getCode() const {
  return _code;
}

std::string const &LocationRedirect::getRedirect() const {
  return _redirect;
}

Task *LocationRedirect::createTask(ftev::StreamConnectionTransport &transport,
                                   ftev::EventLoop::DeferWatcher &complete,
                                   Request const &request) const {
  return new RedirectTask(transport, complete, request, *this);
}
