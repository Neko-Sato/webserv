#include <EventLoop.hpp>

int main() {
  ftev::EventLoop &loop = ftev::EventLoop::default_loop;
  loop.run();
  return 0;
}