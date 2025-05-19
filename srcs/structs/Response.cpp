/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 08:39:11 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/19 21:37:59 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs/Response.hpp"
#include "constants.hpp"
#include "structs/Headers.hpp"

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
  {
    std::string headers;
    composeHeaders(headers, response.headers);
    oss << headers;
  }
  oss << CRLF;
  oss.str().swap(res);
}
