/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 14:32:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/07/12 07:24:41 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>

namespace ftpp {

class URI {
private:
  std::string _scheme;
  std::string _netloc;
  std::string _path;
  std::string _query;
  std::string _fragment;

  static std::string _readScheme(std::string const &url, std::size_t &pos);
  static std::string _readNetloc(std::string const &url, std::size_t &pos);
  static std::string _readPath(std::string const &url, std::size_t &pos);
  static std::string _readQuery(std::string const &url, std::size_t &pos);
  static std::string _readFragment(std::string const &url, std::size_t &pos);

public:
  URI(std::string const &url = "");
  URI(URI const &rhs);
  ~URI();
  URI &operator=(URI const &rhs);

  void swap(URI &rhs) throw();

  std::string const &getScheme() const;
  std::string const &getNetloc() const;
  std::string const &getPath() const;
  std::string const &getQuery() const;
  std::string const &getFragment() const;

  std::string toString() const;
};

std::ostream &operator<<(std::ostream &os, URI const &uri);
std::istream &operator>>(std::istream &is, URI &uri);

} // namespace ftpp

namespace std {

void swap(ftpp::URI &lhs, ftpp::URI &rhs) throw();

} // namespace std
