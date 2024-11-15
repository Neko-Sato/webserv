#include <BaseTimerWatcher.hpp>
#include <BaseIOWatcher.hpp>
#include <EventLoop.hpp>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

class MyTimerWatcher : public ftev::EventLoop::BaseTimerWatcher {
private:
  int _count;

public:
  MyTimerWatcher(ftev::EventLoop &loop)
      : ftev::EventLoop::BaseTimerWatcher(loop), _count(0) {
  }
  ~MyTimerWatcher() {
  }
  void onTimeout() {
    _count++;
    std::cout << "Timer fired!" << std::endl;
    // if (_count == 5)
    //   _loop.stop();
    // else
      start(2000);
  }
};

class MyIOWatcher : public ftev::EventLoop::BaseIOWatcher {
public:
  MyIOWatcher(ftev::EventLoop &loop, char const *filename)
	  : ftev::EventLoop::BaseIOWatcher(loop, open(filename, O_RDONLY)) {
  }
  ~MyIOWatcher() {
  }
  void on_read() {
	char buf[10];
    ssize_t size = read(get_fd(), buf, 10);
    if (size == 0) {
	  close();
      _loop.stop();
	  return;
	}
    size = write(1, buf, size);
  }
  void on_write() {
  }
  void on_error() {
  }
};

int main() {
  ftev::EventLoop &loop = ftev::EventLoop::default_loop;
  MyTimerWatcher timer_watcher(loop);
  MyIOWatcher io_watcher(loop, "./pipe");

  timer_watcher.start(1000);
  io_watcher.start(ftpp::BaseSelector::READ);
  loop.run();
  return 0;
}
