/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Responce.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 08:39:11 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/21 08:41:28 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs/Responce.hpp"
#include "constants.hpp"

#include <algorithm>
#include <sstream>

void Responce::swap(Responce &rhs) throw() {
  version.swap(rhs.version);
  std::swap(status, rhs.status);
  reason.swap(rhs.reason);
  headers.swap(rhs.headers);
}

std::string makeResponce(Responce const &responce) {
  std::ostringstream oss;
  oss << responce.version << " " << responce.status << " " << responce.reason
      << CRLF;
  for (Responce::Headers::const_iterator it = responce.headers.begin();
       it != responce.headers.end(); ++it) {
    for (Responce::HeaderValues::const_iterator jt = it->second.begin();
         jt != it->second.end(); ++jt) {
      oss << it->first << ": " << *jt << CRLF;
    }
  }
  oss << CRLF;
  return oss.str();
}