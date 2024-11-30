/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 08:18:49 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/30 08:46:21 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"

#include <EventLoop.hpp>

#include <iostream>

int main(int argc, char *argv[]) {
  try {
    if (2 < argc)
      throw std::runtime_error("Too many arguments");
    ftev::EventLoop &loop = ftev::EventLoop::default_loop;
    HttpServer server(loop, argc == 2 ? argv[1] : "./configure");
    server.start();
    loop.run();
    return 0;
  } catch (std::exception const &e) {
    std::cerr << argv[0] << ": " << e.what() << std::endl;
    return 1;
  }
}
