/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 07:59:54 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/02 22:57:48 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "HttpServer.hpp"
#include "configs/Configs.hpp"

#include <EventLoop.hpp>
#include <LoopStopper/LoopStopper.hpp>

#include <list>

class Webserv {
public:
  typedef std::list<HttpServer *> Servers;

private:
  Configs _configs;
  ftev::LoopStopper _stopper;
  Servers _servers;

  Webserv();
  Webserv(Webserv const &);
  Webserv &operator=(Webserv const &);

public:
  Webserv(ftev::EventLoop &loop, Configs const &configs);
  ~Webserv();
};
