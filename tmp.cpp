
// #include <Json.hpp>
// #include <JsonParser.hpp>
// #include <Variant.hpp>
// #include <fstream>
// #include <iostream>

// int main() {
//   std::ifstream ifs("./default.json");
//   ftpp::Variant obj = ftjson::JsonParser::parse(ifs);
//   std::cout << obj.as<ftjson::Object>()["servers"]
//                    .as<ftjson::Array>()[0]
//                    .as<ftjson::Object>()["server_names"]
//                    .as<ftjson::Array>()[0]
//                    .as<ftjson::String>()
//             << std::endl;
//   return 0;
// }

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
//   EchoServer server(loop, "::", 8080);
//   server.start();
//   loop.run();
//   std::cout << "Goodbye!" << std::endl;
// }

// /**/