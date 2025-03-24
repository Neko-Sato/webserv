/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:04:54 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 13:56:30 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/ServerConf.hpp"
#include "structs/Address.hpp"

#include <ftjson/Json.hpp>
#include <ftpp/any/Any.hpp>

#include <list>
#include <set>

class Configs {
public:
  typedef std::list<ServerConf> Servers;

private:
  Servers _servers;

  void _takeServers(ftjson::Object const &configs);

public:
  Configs();
  Configs(ftjson::Object const &configs);
  Configs(Configs const &rhs);
  ~Configs();
  Configs &operator=(Configs const &rhs);
  void swap(Configs &rhs) throw();

  static Configs load(std::string const &filename);

  Servers const &getServers() const;

  ServerConf const &findServer(Address const &addr,
                               std::string const &name = "") const;
  std::set<Address> getAddressAll() const;
};
