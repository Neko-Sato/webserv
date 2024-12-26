#include "EchoServer.hpp"

#include <EventLoop.hpp>
#include <EventLoop/BaseSignalWatcher.hpp>

#include <iostream>

class Stopper : public ftev::EventLoop::BaseSignalWatcher {
public:
  Stopper(ftev::EventLoop &loop) : BaseSignalWatcher(loop) {
    start(SIGINT);
  };
  ~Stopper() {
    if (is_active())
      stop();
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
