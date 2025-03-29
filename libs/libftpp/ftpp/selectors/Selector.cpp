/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Selector.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:58:20 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/30 01:20:07 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/selectors/Selector.hpp>

#include <cassert>

namespace ftpp {

Selector::event_t const Selector::READ = 1;
Selector::event_t const Selector::WRITE = 2;
Selector::event_t const Selector::EXCEPT = 4;

Selector::Selector() {
}

Selector::~Selector() {
}

void Selector::add(int fd, event_t events) {
  events &= READ | WRITE;
  events |= EXCEPT;
  if (!_fds.insert(std::make_pair(fd, events)).second)
    throw RegisteredError();
}

void Selector::remove(int fd) {
  Mapping::iterator it = _fds.find(fd);
  if (it == _fds.end())
    throw NotRegisteredError();
  _fds.erase(it);
}

void Selector::modify(int fd, event_t events) {
  Mapping::iterator it = _fds.find(fd);
  if (it == _fds.end())
    throw NotRegisteredError();
  events &= READ | WRITE;
  events |= EXCEPT;
  it->second = events;
}

Selector::Mapping const &Selector::getMap() const {
  return _fds;
}

char const *Selector::RegisteredError::what() const throw() {
  return "already registered";
}

char const *Selector::NotRegisteredError::what() const throw() {
  return "not registered";
}

} // namespace ftpp
