/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseSelector.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 16:35:30 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:55:34 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/selectors/BaseSelector.hpp>

#include <cassert>

namespace ftpp {

BaseSelector::event_t const BaseSelector::READ = 1;
BaseSelector::event_t const BaseSelector::WRITE = 2;
BaseSelector::event_t const BaseSelector::EXCEPT = 4;

BaseSelector::BaseSelector() {
}

BaseSelector::~BaseSelector() {
}

void BaseSelector::add(int fd, event_t events) {
  if (!_fds.insert(std::make_pair(fd, events & (READ | WRITE))).second)
    throw RegisteredError();
}

void BaseSelector::remove(int fd) {
  Mapping::iterator it = _fds.find(fd);
  if (it == _fds.end())
    throw NotRegisteredError();
  _fds.erase(it);
}

void BaseSelector::modify(int fd, event_t events) {
  Mapping::iterator it = _fds.find(fd);
  if (it == _fds.end())
    throw NotRegisteredError();
  it->second = events & (READ | WRITE);
}

BaseSelector::Mapping const &BaseSelector::get_map() const {
  return _fds;
}

char const *BaseSelector::RegisteredError::what() const throw() {
  return "File descriptor already registered for selector.";
}

char const *BaseSelector::NotRegisteredError::what() const throw() {
  return "File descriptor not registered in selector.";
}

} // namespace ftpp
