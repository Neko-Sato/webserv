/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AddrInfos.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 04:39:48 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/01 23:33:16 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/macros.hpp>
#include <ftpp/socket/AddrInfos.hpp>

#include <cstring>

namespace ftpp {

AddrInfos::iterator::iterator(addrinfo const *info) : _current(info) {
}
AddrInfos::iterator::iterator(iterator const &rhs) : _current(rhs._current) {
}

AddrInfos::iterator::~iterator() {
}

AddrInfos::iterator &AddrInfos::iterator::operator=(iterator const &rhs) {
  if (this != &rhs) {
    _current = rhs._current;
  }
  return *this;
}

AddrInfos::iterator &AddrInfos::iterator::operator++() {
  _current = _current->ai_next;
  return *this;
}

AddrInfos::iterator AddrInfos::iterator::operator++(int) {
  iterator tmp(*this);
  _current = _current->ai_next;
  return tmp;
}

bool AddrInfos::iterator::operator==(iterator const &rhs) const {
  return _current == rhs._current;
}

bool AddrInfos::iterator::operator!=(iterator const &rhs) const {
  return _current != rhs._current;
}

addrinfo const &AddrInfos::iterator::operator*() const {
  return *_current;
}

addrinfo const *AddrInfos::iterator::operator->() const {
  return _current;
}

AddrInfos::Hints::Hints() {
  memset(static_cast<addrinfo *>(this), 0, sizeof(addrinfo));
}

AddrInfos::Hints::Hints(int family, int socktype, int protocol, int flags) {
  memset(static_cast<addrinfo *>(this), 0, sizeof(addrinfo));
  ai_family = family;
  ai_socktype = socktype;
  ai_protocol = protocol;
  ai_flags = flags;
}

AddrInfos::Hints::Hints(Hints const &rhs) {
  *static_cast<addrinfo *>(this) = static_cast<addrinfo const &>(rhs);
}

AddrInfos::Hints::~Hints() {
}

AddrInfos::Hints &AddrInfos::Hints::operator=(Hints const &rhs) {
  if (this != &rhs) {
    *static_cast<addrinfo *>(this) = static_cast<addrinfo const &>(rhs);
  }
  return *this;
}

addrinfo const *AddrInfos::Hints::get_addrinfo() const {
  return static_cast<addrinfo const *>(this);
}

AddrInfos::GAIError::GAIError(int __errno)
    : _errno(__errno), _s(std::string("getaddrinfo: ") + gai_strerror(_errno)) {
}

AddrInfos::GAIError::GAIError(GAIError const &rhs)
    : _errno(rhs._errno), _s(rhs._s) {
}

AddrInfos::GAIError::~GAIError() throw() {
}

AddrInfos::GAIError &AddrInfos::GAIError::operator=(GAIError const &rhs) {
  if (this != &rhs) {
    _errno = rhs._errno;
    _s = rhs._s;
  }
  return *this;
}

int AddrInfos::GAIError::getErrno() const {
  return _errno;
}

char const *AddrInfos::GAIError::what() const throw() {
  return _s.c_str();
}

addrinfo *AddrInfos::_getaddrinfo(char const *name, char const *service,
                                  addrinfo const *hints) {
  addrinfo *res;
  int err = ::getaddrinfo(name, service, hints, &res);
  if (unlikely(err != 0))
    throw GAIError(err);
  return res;
}

AddrInfos::AddrInfos(char const *name, char const *service) {
  _addrinfo = _getaddrinfo(name, service);
}

AddrInfos::AddrInfos(char const *name, char const *service,
                     Hints const &hints) {
  _addrinfo = _getaddrinfo(name, service, hints.get_addrinfo());
}

AddrInfos::~AddrInfos() {
  freeaddrinfo(_addrinfo);
}

AddrInfos::iterator AddrInfos::begin() const {
  return iterator(_addrinfo);
}

AddrInfos::iterator AddrInfos::end() const {
  return iterator(NULL);
}

} // namespace ftpp
