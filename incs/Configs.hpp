/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configs.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 14:04:54 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/30 18:38:02 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

#include <Any.hpp>

#include <list>

class Configs {
public:
  typedef std::list<Server> Servers;

private:
  Servers _servers;

  static Servers _takeServers(ftpp::Any const &value);

public:
  Configs();
  Configs(ftpp::Any const &value);
  Configs(Configs const &rhs);
  Configs &operator=(Configs const &rhs);
  ~Configs();

  Servers const &getServers() const;

  static Configs load(std::string const &filename);
};
