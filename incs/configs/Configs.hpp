/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:04:54 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 00:20:05 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/ServerConf.hpp"
#include "structs/address.hpp"

#include <Any.hpp>
#include <Json.hpp>

#include <list>
#include <set>

class Configs {
public:
  typedef std::list<ServerConf> Servers;
  typedef std::set<address> Addresses;

private:
  Servers _servers;

  void _takeServers(ftjson::Object const &configs);

public:
  Configs();
  Configs(ftpp::Any const &value);
  Configs(Configs const &rhs);
  Configs &operator=(Configs const &rhs);
  ~Configs();

  static Configs load(std::string const &filename);

  Addresses getAllAddresses() const;
};
