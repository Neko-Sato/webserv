/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathjoin.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uakizuki <uakizuki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 19:58:08 by uakizuki          #+#    #+#             */
/*   Updated: 2025/07/12 20:02:38 by uakizuki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftpp/pathlib/pathlib.hpp>

namespace ftpp {

std::string pathjoin(std::string const &path1, std::string const &path2) {
  std::string tmp(path1);
  if (!tmp.empty() && *tmp.cbegin() == '/')
    tmp += '/';
  tmp += path2;
  return normpath(tmp);
}

} // namespace ftpp
