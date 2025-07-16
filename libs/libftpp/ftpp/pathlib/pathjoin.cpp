/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathjoin.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 19:58:08 by uakizuki          #+#    #+#             */
/*   Updated: 2025/07/16 23:13:13 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/pathlib/pathlib.hpp>

namespace ftpp {

std::string pathjoin(std::string const &path1, std::string const &path2) {
  std::string tmp(path1);
  if (!tmp.empty() && *tmp.rbegin() != '/')
    tmp += '/';
  tmp += path2;
  return normpath(tmp);
}

} // namespace ftpp
