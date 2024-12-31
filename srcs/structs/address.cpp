/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   address.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 09:45:58 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 00:24:03 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs/address.hpp"

#include <string>

address::address(std::string const &host, int port) : host(host), port(port) {
}

bool address::operator<(address const &rhs) const {
  return host < rhs.host || (host == rhs.host && port < rhs.port);
}
