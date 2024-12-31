/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   address.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 09:45:58 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/31 23:35:21 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

struct address {
  std::string host;
  int port;

  address(std::string const &host, int port);
  bool operator<(address const &rhs) const;
};
