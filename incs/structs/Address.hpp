/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Address.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 09:45:58 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/24 21:47:01 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <Json.hpp>

#include <string>

struct Address {
  std::string host;
  int port;

  static int const PORT_MIN;
  static int const PORT_MAX;

  Address(std::string const &host, int port);
  Address(ftjson::Object const &addr);

  void swap(Address &rhs) throw();
  bool operator<(Address const &rhs) const;
};
