/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 08:30:00 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/30 08:45:25 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop.hpp>

#include "Configs.hpp"

class HttpServer {
private:
  ftev::EventLoop &_loop;
  Configs _configs;

  HttpServer(HttpServer const &);
  HttpServer &operator=(HttpServer const &);

public:
  HttpServer(ftev::EventLoop &loop, Configs const &configs);
  ~HttpServer();

  void start();
};
