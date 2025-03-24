/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 07:59:54 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 13:58:48 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "configs/Configs.hpp"

#include <ftev/EventLoop.hpp>
#include <ftev/LoopStopper/LoopStopper.hpp>
#include <ftpp/noncopyable/NonCopyable.hpp>

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
