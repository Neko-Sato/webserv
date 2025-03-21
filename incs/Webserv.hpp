/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 07:59:54 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/05 01:18:41 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "configs/Configs.hpp"

#include <EventLoop.hpp>
#include <LoopStopper/LoopStopper.hpp>
#include <NonCopyable.hpp>

#include <list>

class Webserv : private ftpp::NonCopyable {
public:
  typedef std::list<Server *> Servers;

private:
  Configs _configs;
  ftev::LoopStopper _stopper;
  Servers _servers;

  Webserv();

public:
  Webserv(ftev::EventLoop &loop, Configs const &configs);
  ~Webserv();
};
