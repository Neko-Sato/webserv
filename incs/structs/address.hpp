/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   address.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 09:45:58 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/08 01:28:55 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Json.hpp>

#include <string>

struct address {
  std::string host;
  int port;

  address(std::string const &host, int port);
  address(ftjson::Object const &addr);
  bool operator<(address const &rhs) const;
};
