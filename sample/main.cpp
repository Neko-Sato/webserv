/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 02:27:01 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/03 02:32:59 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EchoServer.hpp"

#include <EventLoop.hpp>

#include <iostream>

int main() {
  try {
    ftev::EventLoop &loop = ftev::EventLoop::default_loop;
    EchoServer server(loop, "0.0.0.0", 8080);
    loop.run();
    return 0;
  } catch (std::exception &e) {
    std::cerr << "echho server: " << e.what() << std::endl;
    return 1;
  }
}