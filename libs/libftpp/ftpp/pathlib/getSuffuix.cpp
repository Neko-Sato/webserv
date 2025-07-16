/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getsuffuix.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 21:02:30 by uakizuki          #+#    #+#             */
/*   Updated: 2025/07/16 21:03:27 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

namespace ftpp {

std::string getSuffix(const std::string &path) {
  std::string::size_type slash_pos = path.find_last_of('/');
  std::string filename =
      slash_pos == std::string::npos ? path : path.substr(slash_pos + 1);
  std::string::size_type dot_pos = filename.find_last_of('.');
  if (dot_pos == std::string::npos)
    return "";
  return filename.substr(dot_pos);
}

} // namespace ftpp
