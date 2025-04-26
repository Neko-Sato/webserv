/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isAbsolutePath.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 02:51:22 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/27 03:58:44 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

namespace ftpp {

bool isAbsolutePath(std::string const &path) {
  return !path.empty() && path[0] == '/';
}

} // namespace ftpp
