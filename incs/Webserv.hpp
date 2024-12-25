/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 07:59:54 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/26 08:09:30 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Configs.hpp"

#include <EventLoop.hpp>

#ifndef DEFAULT_CONFIGURE
#define DEFAULT_CONFIGURE "./default.json"
#endif

class Webserv {
private:
  ftev::EventLoop &_loop;
  Configs _configs;

  Webserv();
  Webserv(Webserv const &);
  Webserv &operator=(Webserv const &);

public:
  Webserv(ftev::EventLoop &loop, Configs const &configs);
  ~Webserv();
};