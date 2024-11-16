#include <EventLoop/BaseIOWatcher.hpp>
#include <EventLoop/BaseSignalWatcher.hpp>
#include <EventLoop/BaseTimerWatcher.hpp>
#include <EventLoop/BaseProcessWatcher.hpp>
#include <EventLoop.hpp>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

class MyTimerWatcher : public ftev::EventLoop::BaseTimerWatcher {
private:
  int _count;

public:
  MyTimerWatcher(ftev::EventLoop &loop)
      : ftev::EventLoop::BaseTimerWatcher(loop), _count(0) {
  }
  ~MyTimerWatcher() {
  }
  void on_timeout() {
    _count++;
    std::cout << "Timer fired!" << std::endl;
    // if (_count == 5)
    //   _loop.stop();
    // else
    start(100);
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

class MySignalWatcher : public ftev::EventLoop::BaseSignalWatcher {
public:
  MySignalWatcher(ftev::EventLoop &loop)
	  : ftev::EventLoop::BaseSignalWatcher(loop) {
  }
  ~MySignalWatcher() {
  }
  void on_signal() {
	std::cout << "Signal received!" << std::endl;
  }
};

class MyProcessWatcher : public ftev::EventLoop::BaseProcessWatcher {
public:
  MyProcessWatcher(ftev::EventLoop &loop)
	  : ftev::EventLoop::BaseProcessWatcher(loop) {
  }
  ~MyProcessWatcher() {
  }
  void on_exit(int status) {
	std::cout << "Process exited! " << status << std::endl;
  }
};

#include <unistd.h>

int main() {
  pid_t pid = fork();
  if (pid == 0) {
    char const *const argv[] = {"python", "-m", "http.server", NULL};
	execve("/bin/python", const_cast<char* const*>(argv), environ);
  }
  ftev::EventLoop &loop = ftev::EventLoop::default_loop;
  MyTimerWatcher timer_watcher(loop);
  MyIOWatcher io_watcher(loop, "./pipe");
  MySignalWatcher signal_watcher(loop);
  MyProcessWatcher process_watcher(loop);

  timer_watcher.start(100);
  io_watcher.start(ftpp::BaseSelector::READ);
  signal_watcher.start(SIGINT);
  process_watcher.start(pid);
  loop.run();
  return 0;
}
