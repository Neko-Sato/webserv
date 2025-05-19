/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 02:12:43 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/16 06:38:16 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs/Request.hpp"
#include "constants.hpp"

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
  bool is_first_line = true;
  Request tmp;
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
      iss >> tmp.method >> tmp.uri >> tmp.version;
      if (iss.fail())
        throw std::runtime_error("Invalid request");
    } else {
      std::string::size_type pos = line.find(":");
      if (pos == std::string::npos)
        throw std::runtime_error("Invalid request");
      tmp.headers[ftpp::tolower(ftpp::strtrim(line.substr(0, pos)))].push_back(
          ftpp::strtrim(line.substr(pos + 1)));
    }
    if (it == data.end())
      break;
  }
  std::cerr << "Request: " << tmp.method << "\n " << tmp.uri.toString() << "\n "
			<< tmp.version << std::endl;
  res.swap(tmp);

}
