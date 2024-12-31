/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 07:59:54 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/31 10:43:32 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Configs.hpp"
#include "SigIntHandler.hpp"

#include <EventLoop.hpp>

class Webserv {
private:
  Configs _configs;
  SigIntHandler _sigint_handler;

  Webserv();
  Webserv(Webserv const &);
  Webserv &operator=(Webserv const &);

public:
  Webserv(ftev::EventLoop &loop, Configs const &configs);
  ~Webserv();
};
