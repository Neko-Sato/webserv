#include <ftev/EventLoop.hpp>
#include <ftev/LoopStopper/LoopStopper.hpp>
#include <ftev/Sample/EchoServer/EchoServer.hpp>
#include <ftev/Sample/SimpleClient/SimpleClient.hpp>

int main() {
  ftev::EventLoop &loop = ftev::EventLoop::defaultLoop;
  ftev::LoopStopper stopper(loop);
  ftev::EchoServer server(loop, "0.0.0.0", 8080);
  ftev::SimpleClient client(loop, "localhost", 8081);
  loop.run();
  return 0;
}
