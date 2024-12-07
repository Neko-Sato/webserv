/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 08:18:49 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/07 20:30:15 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EchoServer.hpp"

#include <EventLoop.hpp>
#include <EventLoop/BaseSignalWatcher.hpp>

#include <iostream>

class Stopper : public ftev::EventLoop::BaseSignalWatcher {
public:
  Stopper(ftev::EventLoop &loop) : BaseSignalWatcher(loop) {
    start(SIGINT);
  };
  void on_signal() {
    loop.stop();
  };
};

int main() {
  ftev::EventLoop &loop = ftev::EventLoop::default_loop;
  Stopper stopper(loop);
  EchoServer server(loop, "::", 8080);
  server.start();
  loop.run();
  std::cout << "Goodbye!" << std::endl;
}

// #include "Webserv.hpp"

// #include <EventLoop.hpp>

// #include <iostream>

// int main(int argc, char *argv[]) {
//   try {
//     if (2 < argc)
//       throw std::runtime_error("Too many arguments");
//     ftev::EventLoop &loop = ftev::EventLoop::default_loop;
//     Webserv webserv(loop, argc == 2 ? argv[1] : "./configure");
//     loop.run();
//     return 0;
//   } catch (std::exception const &e) {
//     std::cerr << argv[0] << ": " << e.what() << std::endl;
//     return 1;
//   }
// }
