/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationDefault.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 17:21:52 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/30 18:50:20 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Locations/LocationDefault.hpp"

#include <stdexcept>

LocationDefault::LocationDefault(ftpp::Any const &value)
    : BaseLocation(value), _root(_takeRoot(value)), _index(_takeIndex(value)),
      _autoindex(_takeAutoindex(value)), _cgi(_takeCgi(value)) {
}

LocationDefault::LocationDefault(LocationDefault const &rhs)
    : BaseLocation(rhs) , _root(rhs._root), _index(rhs._index), _autoindex(rhs._autoindex),
      _cgi(rhs._cgi) {
}

LocationDefault &LocationDefault::operator=(LocationDefault const &rhs) {
  if (this != &rhs) {
    BaseLocation::operator=(rhs);
    _root = rhs._root;
    _index = rhs._index;
    _autoindex = rhs._autoindex;
    _cgi = rhs._cgi;
  }
  return *this;
}

LocationDefault::~LocationDefault() {
}

LocationDefault *LocationDefault::copy() const {
  return new LocationDefault(*this);
}
