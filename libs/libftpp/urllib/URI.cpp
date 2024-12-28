/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:50:24 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/28 20:56:48 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <urllib/URI.hpp>

#include <cstring>
#include <sstream>

namespace ftpp {

URI::URI(std::string const &url) {
  std::size_t pos = 0;
  _scheme = _readScheme(url, pos);
  _netloc = _readNetloc(url, pos);
  _path = _readPath(url, pos);
  _query = _readQuery(url, pos);
  _fragment = _readFragment(url, pos);
}

URI::URI(URI const &rhs)
    : _scheme(rhs._scheme), _netloc(rhs._netloc), _path(rhs._path),
      _query(rhs._query), _fragment(rhs._fragment) {
}

URI::~URI() {
}

URI &URI::operator=(URI const &rhs) {
  if (this != &rhs) {
    _scheme = rhs._scheme;
    _netloc = rhs._netloc;
    _path = rhs._path;
    _query = rhs._query;
    _fragment = rhs._fragment;
  }
  return *this;
}

std::string URI::toString() const {
  std::stringstream tmp;
  if (!_scheme.empty())
    tmp << _scheme << ":";
  if (!_netloc.empty())
    tmp << "//" << _netloc;
  if (!_path.empty())
    tmp << _path;
  if (!_query.empty())
    tmp << "?" << _query;
  if (!_fragment.empty())
    tmp << "#" << _fragment;
  return tmp.str();
}

std::string URI::_readScheme(std::string const &url, std::size_t &pos) {
  std::size_t _pos = pos;
  if (_pos >= url.size() || memchr(":/?#", url[_pos], 4))
    return "";
  while (_pos < url.size() && !memchr(":/?#", url[_pos], 4))
    ++_pos;
  if (_pos >= url.size() || url[_pos] != ':')
    return "";
  std::string result = url.substr(pos, _pos - pos);
  ++_pos;
  pos = _pos;
  return result;
}

std::string URI::_readNetloc(std::string const &url, std::size_t &pos) {
  std::size_t _pos = pos;
  if (url.substr(_pos, 2) != "//")
    return "";
  _pos += 2;
  while (_pos < url.size() && !memchr("/?#", url[_pos], 3))
    ++_pos;
  std::string result = url.substr(pos + 2, _pos - pos - 2);
  pos = _pos;
  return result;
}

std::string URI::_readPath(std::string const &url, std::size_t &pos) {
  std::size_t _pos = pos;
  while (_pos < url.size() && !memchr("?#", url[_pos], 3))
    ++_pos;
  std::string result = url.substr(pos, _pos - pos);
  pos = _pos;
  return result;
}

std::string URI::_readQuery(std::string const &url, std::size_t &pos) {
  std::size_t _pos = pos;
  if (_pos >= url.size() || url[_pos] != '?')
    return "";
  ++_pos;
  while (_pos < url.size() && !memchr("#", url[_pos], 1))
    ++_pos;
  std::string result = url.substr(pos + 1, _pos - pos);
  pos = _pos;
  return result;
}

std::string URI::_readFragment(std::string const &url, std::size_t &pos) {
  std::size_t _pos = pos;
  if (_pos >= url.size() || url[_pos] != '#')
    return "";
  std::string result = url.substr(pos + 1);
  pos = url.size();
  return result;
}

std::string const &URI::getScheme() const {
  return _scheme;
}

std::string const &URI::getNetloc() const {
  return _netloc;
}

std::string const &URI::getPath() const {
  return _path;
}

std::string const &URI::getQuery() const {
  return _query;
}

std::string const &URI::getFragment() const {
  return _fragment;
}

void URI::setScheme(std::string const &scheme) {
  if (scheme.find_first_of(":/?#") != std::string::npos)
    throw std::invalid_argument("Invalid scheme");
  _scheme = scheme;
}

void URI::setNetloc(std::string const &netloc) {
  if (netloc.find_first_of("/?#") != std::string::npos)
    throw std::invalid_argument("Invalid netloc");
  _netloc = netloc;
}

void URI::setPath(std::string const &path) {
  if (path.find_first_of("?#") != std::string::npos)
    throw std::invalid_argument("Invalid path");
  _path = path;
}

void URI::setQuery(std::string const &query) {
  if (query.find_first_of("#") != std::string::npos)
    throw std::invalid_argument("Invalid query");
  _query = query;
}

void URI::setFragment(std::string const &fragment) {
  _fragment = fragment;
}

} // namespace ftpp
