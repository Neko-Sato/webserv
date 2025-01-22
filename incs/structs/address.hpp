/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   address.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 09:45:58 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/22 07:30:26 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Any.hpp>

#include <string>

struct address {
  std::string host;
  int port;

  address(std::string const &host, int port);
  address(ftpp::Any const &value);
  bool operator<(address const &rhs) const;
};
