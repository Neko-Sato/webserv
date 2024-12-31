/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 08:18:49 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/31 23:00:53 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include <EventLoop.hpp>

#include <cstdlib>
#include <iostream>

#ifndef DEFAULT_CONFIGURE
#define DEFAULT_CONFIGURE "./default.json"
#endif

int main(int argc, char *argv[]) {
  //   try {
  if (2 < argc)
    throw std::runtime_error("Too many arguments");
  ftev::EventLoop &loop = ftev::EventLoop::default_loop;
  char const *const &config_path = argc == 2 ? argv[1] : DEFAULT_CONFIGURE;
  Webserv webserv(loop, Configs::load(config_path));
  loop.run();
  return EXIT_SUCCESS;
  //   } catch (std::exception const &e) {
  //     char const *name = 1 <= argc ? argv[0] : "wevserv";
  //     std::cerr << name << ": " << e.what() << std::endl;
  //     return EXIT_FAILURE;
  //   }
}
