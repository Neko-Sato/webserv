/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 08:39:11 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 02:15:23 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs/Response.hpp"
#include "constants.hpp"

#include <ftpp/string/string.hpp>

#include <algorithm>
#include <sstream>

void Response::swap(Response &rhs) throw() {
  version.swap(rhs.version);
  std::swap(status, rhs.status);
  reason.swap(rhs.reason);
  headers.swap(rhs.headers);
}

void composeResponse(std::string &res, Response const &response) {
  std::ostringstream oss;
  oss << response.version << " " << response.status << " " << response.reason
      << CRLF;
  for (Response::Headers::const_iterator it = response.headers.begin();
       it != response.headers.end(); ++it) {
    for (Response::HeaderValues::const_iterator jt = it->second.begin();
         jt != it->second.end(); ++jt) {
      oss << ftpp::tolower(it->first) << ": " << *jt << CRLF;
    }
  }
  oss << CRLF;
  res = oss.str();
}
