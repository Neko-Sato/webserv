/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normpath.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 02:05:47 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/27 03:59:59 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/string/string.hpp>

#include <string>
#include <vector>

namespace ftpp {

std::string normpath(std::string const &path) {
  int initialSlashCount = 0;
  for (std::size_t i = 0; i < 3 && i < path.size() && path[i] == '/'; ++i)
    ++initialSlashCount;
  initialSlashCount = 2 < initialSlashCount ? 1 : initialSlashCount;
  std::vector<std::string> parts;
  ftpp::strsplit(path, "/", std::back_inserter(parts));
  for (std::vector<std::string>::iterator it = parts.begin();
       it != parts.end();) {
    if (*it == "." || *it == "")
      it = parts.erase(it);
    else if (*it == "..") {
      it = parts.erase(it);
      if (it != parts.begin())
        it = parts.erase(--it);
    } else
      ++it;
  }
  return std::string(initialSlashCount, '/') +
         ftpp::strjoin(parts.begin(), parts.end(), "/");
}

} // namespace ftpp
