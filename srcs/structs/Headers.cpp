/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 21:30:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/19 22:26:33 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs/Headers.hpp"
#include "constants.hpp"

#include <ftpp/string/string.hpp>

#include <algorithm>
#include <sstream>
#include <stdexcept>

void parseHeaders(Headers &res, std::string const &data) {
  Headers tmp;
  std::string::const_iterator it = data.begin();
  for (;;) {
    std::string::const_iterator match =
        std::search(it, data.end(), CRLF.begin(), CRLF.end());
    if (match == data.end())
      throw std::runtime_error("Invalid Header");
    std::string line(it, match);
    it = match + CRLF.size();
    if (line.empty())
      break;
    std::string::size_type pos = line.find(":");
    if (pos == std::string::npos)
      throw std::runtime_error("Invalid Header");
    tmp[ftpp::tolower(ftpp::strtrim(line.substr(0, pos)))].push_back(
        ftpp::strtrim(line.substr(pos + 1)));
    if (it == data.end())
      break;
  }
  res.swap(tmp);
}

void composeHeaders(std::string &res, Headers const &headers) {
  std::ostringstream oss;
  for (Headers::const_iterator it = headers.begin(); it != headers.end();
       ++it) {
    for (HeaderValues::const_iterator jt = it->second.begin();
         jt != it->second.end(); ++jt)
      oss << ftpp::tolower(it->first) << ": " << *jt << CRLF;
  }
  oss.str().swap(res);
}
