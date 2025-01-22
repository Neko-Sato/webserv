/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 07:59:54 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/23 06:33:21 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "configs/Configs.hpp"

#include <EventLoop.hpp>
#include <LoopStopper/LoopStopper.hpp>

#include <list>

class Webserv {
private:
  Configs _configs;
  ftev::LoopStopper _stopper;

  Webserv();
  Webserv(Webserv const &);
  Webserv &operator=(Webserv const &);

public:
  Webserv(ftev::EventLoop &loop, Configs const &configs);
  ~Webserv();
};
