/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:50:24 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 17:55:34 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/urllib/URI.hpp>

#include <cstring>
#include <sstream>

namespace ftpp {

URI::URI(std::string const &url) {
  std::size_t pos = 0;
  _readScheme(url, pos).swap(_scheme);
  _readNetloc(url, pos).swap(_netloc);
  _readPath(url, pos).swap(_path);
  _readQuery(url, pos).swap(_query);
  _readFragment(url, pos).swap(_fragment);
}

URI::URI(URI const &rhs)
    : _scheme(rhs._scheme), _netloc(rhs._netloc), _path(rhs._path),
      _query(rhs._query), _fragment(rhs._fragment) {
}

URI::~URI() {
}

URI &URI::operator=(URI const &rhs) {
  if (this != &rhs)
    URI(rhs).swap(*this);
  return *this;
}

void URI::swap(URI &rhs) throw() {
  _scheme.swap(rhs._scheme);
  _netloc.swap(rhs._netloc);
  _path.swap(rhs._path);
  _query.swap(rhs._query);
  _fragment.swap(rhs._fragment);
}

std::string URI::toString() const {
  std::ostringstream tmp;
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
  std::size_t _pos = url.find_first_of(":/?#", pos);
  if (_pos == std::string::npos)
    return "";
  if (url[_pos] != ':')
    return "";
  std::string result(url, pos, _pos - pos);
  ++_pos;
  pos = _pos;
  return result;
}

std::string URI::_readNetloc(std::string const &url, std::size_t &pos) {
  if (url.compare(pos, 2, "//"))
    return "";
  std::size_t _pos = url.find_first_of("/?#", pos + 2);
  std::string result(url, pos + 2, _pos - (pos + 2));
  pos = _pos;
  return result;
}

std::string URI::_readPath(std::string const &url, std::size_t &pos) {
  std::size_t _pos = url.find_first_of("?#", pos);
  _pos = _pos == std::string::npos ? url.size() : _pos;
  std::string result(url, pos, _pos - pos);
  pos = _pos;
  return result;
}

std::string URI::_readQuery(std::string const &url, std::size_t &pos) {
  if (url.compare(pos, 1, "?"))
    return "";
  std::size_t _pos = url.find_first_of("#", pos + 1);
  _pos = _pos == std::string::npos ? url.size() : _pos;
  std::string result(url, pos + 1, _pos - (pos + 1));
  pos = _pos;
  return result;
}

std::string URI::_readFragment(std::string const &url, std::size_t &pos) {
  if (url.compare(pos, 1, "#"))
    return "";
  std::string result(url, pos + 1);
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

std::ostream &operator<<(std::ostream &os, URI const &uri) {
  return os << uri.toString();
}

std::istream &operator>>(std::istream &is, URI &uri) {
  std::string tmp;
  is >> tmp;
  if (!is.fail())
    URI(tmp).swap(uri);
  return is;
}

} // namespace ftpp
