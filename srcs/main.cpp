/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 08:18:49 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/24 21:25:40 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

#include <ftev/EventLoop.hpp>

#include <cstdlib>
#include <iostream>

static inline char const *get_default_config_path() {
  char const *const config_path = getenv("WEBSERV_CONFIG");
  return config_path ? config_path : "./default.json";
}

int main(int argc, char *argv[]) {
  try {
    if (2 < argc)
      throw std::runtime_error("Too many arguments");
    char const *const &config_path =
        argc == 2 ? argv[1] : get_default_config_path();
    ftev::EventLoop loop;
    Webserv webserv(loop, Configs::load(config_path));
    loop.run();
    return EXIT_SUCCESS;
  } catch (std::exception const &e) {
    char const *name = 1 <= argc ? argv[0] : "wevserv";
    std::cerr << name << ": " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
