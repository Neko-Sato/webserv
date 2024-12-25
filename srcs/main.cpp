/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 08:18:49 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/25 08:24:10 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Json.hpp>
#include <JsonArray.hpp>
#include <JsonObject.hpp>
#include <JsonParser.hpp>
#include <JsonString.hpp>
#include <fstream>
#include <iostream>

int main() {
  std::ifstream ifs("./default.json");
  ftjson::Json obj = ftjson::JsonParser::parse(ifs);
  std::cout << obj.as<ftjson::JsonObject>()
                   .value["servers"]
                   .as<ftjson::JsonArray>()
                   .value[0]
                   .as<ftjson::JsonObject>()
                   .value["server_name"]
                   .as<ftjson::JsonArray>()
                   .value[0]
                   .as<ftjson::JsonString>()
                   .value
            << std::endl;
  return 0;
}

/**/

// #include "EchoServer.hpp"

// #include <EventLoop.hpp>
// #include <EventLoop/BaseSignalWatcher.hpp>

// #include <iostream>

// class Stopper : public ftev::EventLoop::BaseSignalWatcher {
// public:
//   Stopper(ftev::EventLoop &loop) : BaseSignalWatcher(loop) {
//     start(SIGINT);
//   };
//   ~Stopper() {
//     if (is_active())
//       stop();
//   };
//   void on_signal() {
//     loop.stop();
//   };
// };

// int main() {
//   ftev::EventLoop &loop = ftev::EventLoop::default_loop;
//   Stopper stopper(loop);
//   EchoServer server(loop, "0.0.0.0", 8080);
//   server.start();
//   loop.run();
//   std::cout << "Goodbye!" << std::endl;
// }

/**/

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
