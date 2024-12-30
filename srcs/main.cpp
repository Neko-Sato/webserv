/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 08:18:49 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/30 03:15:58 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include <EventLoop.hpp>

#include <iostream>

int main(int argc, char *argv[]) {
  try {
    if (2 < argc)
      throw std::runtime_error("Too many arguments");
    ftev::EventLoop &loop = ftev::EventLoop::default_loop;
    char const *const &config_path = argc == 2 ? argv[1] : DEFAULT_CONFIGURE;
    Webserv webserv(loop, Configs::load(config_path));
    loop.run();
    return 0;
  } catch (std::exception const &e) {
    std::cerr << argv[0] << ": " << e.what() << std::endl;
    return 1;
  }
}
