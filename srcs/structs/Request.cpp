/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 02:12:43 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/02 10:08:59 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs/Request.hpp"
#include "constants.hpp"

#include <ft_string.hpp>

#include <algorithm>
#include <sstream>
#include <stdexcept>

void Request::swap(Request &rhs) throw() {
  method.swap(rhs.method);
  path.swap(rhs.path);
  version.swap(rhs.version);
  headers.swap(rhs.headers);
}

Request parseRequest(std::string const &data) {
  bool is_first_line = true;
  Request req;
  std::string::const_iterator it = data.begin();
  for (;;) {
    std::string::const_iterator match =
        std::search(it, data.end(), CRLF.begin(), CRLF.end());
    if (match == data.end())
      throw std::runtime_error("Invalid request");
    std::string line(it, match);
    it = match + CRLF.size();
    if (line.empty())
      break;
    if (is_first_line) {
      is_first_line = false;
      std::istringstream iss(line);
      ftpp::URI uri;
      iss >> req.method >> uri >> req.version;
      if (iss.fail())
        throw std::runtime_error("Invalid request");
      req.path = uri.getPath();
      req.query = uri.getQuery();
    } else {
      std::string::size_type pos = line.find(":");
      if (pos == std::string::npos)
        throw std::runtime_error("Invalid request");
      req.headers[ftpp::tolower(ftpp::strtrim(line.substr(0, pos)))].push_back(
          ftpp::strtrim(line.substr(pos + 1)));
    }
    if (it == data.end())
	  break;
  }
  return req;
}
