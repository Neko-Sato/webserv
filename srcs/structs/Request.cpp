/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 02:12:43 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/19 21:44:56 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs/Request.hpp"
#include "constants.hpp"
#include "structs/Headers.hpp"

#include <ftpp/string/string.hpp>

#include <algorithm>
#include <sstream>
#include <stdexcept>

void Request::swap(Request &rhs) throw() {
  method.swap(rhs.method);
  uri.swap(rhs.uri);
  version.swap(rhs.version);
  headers.swap(rhs.headers);
}

void parseRequest(Request &res, std::string const &data) {
  Request tmp;
  std::string::const_iterator match =
      std::search(data.begin(), data.end(), CRLF.begin(), CRLF.end());
  if (match == data.end())
    throw std::runtime_error("Invalid request");
  {
    std::string line(data.begin(), match);
    std::istringstream iss(line);
    ftpp::URI uri;
    iss >> tmp.method >> tmp.uri >> tmp.version;
    if (iss.fail())
      throw std::runtime_error("Invalid request");
  }
  parseHeaders(tmp.headers, std::string(match + CRLF.size(), data.end()));
  res.swap(tmp);
}
