#include <EventLoop.hpp>
#include <Watchers/IOWatcher.hpp>
#include <Watchers/ProcessWatcher.hpp>
#include <Watchers/SignalWatcher.hpp>
#include <Watchers/TimerWatcher.hpp>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

void on_timeout(ftev::EventLoop::BaseTimerWatcher &watcher, int _) {
  (void)_;
  std::cout << "Timer fired!" << std::endl;
  watcher.start(1000);
}

void on_read(ftev::EventLoop::BaseIOWatcher &watcher, int _) {
  (void)_;
  char buf[10];
  ssize_t size = read(watcher.get_fd(), buf, 10);
  if (size == 0) {
    return;
  }
  size = write(1, buf, size);
}

void on_signal(ftev::EventLoop::BaseSignalWatcher &watcher, int _) {
  (void)_;
  (void)watcher;
  std::cout << "Signal received!" << std::endl;
}

void on_exit(ftev::EventLoop::BaseProcessWatcher &watcher, int status, int _) {
  (void)_;
  (void)watcher;
  std::cout << "Process exited! " << status << std::endl;
  watcher.loop.stop();
}

#include <unistd.h>

int main() {
  pid_t pid = fork();
  if (pid == 0) {
    char const *const argv[] = {
        "python", "-c", "from time import sleep; sleep(2000); print('Bye!')",
        NULL};
    execve("/bin/python", const_cast<char *const *>(argv), environ);
  }
  std::cout << pid << std::endl;
  ftev::EventLoop &loop = ftev::EventLoop::default_loop;

  ftev::ProcessWatcher<int> process_watcher(loop, on_exit, 0);
  ftev::TimerWatcher<int> timer_watcher(loop, on_timeout, 0);
  ftev::IOWatcher<int> io_watcher(loop, on_read, NULL, NULL, 0);
  ftev::SignalWatcher<int> signal_watcher(loop, on_signal, 0);

  int fd = open("./pipe", O_RDONLY);
  timer_watcher.start(1000);
  io_watcher.start(fd, ftpp::BaseSelector::READ);
  signal_watcher.start(SIGINT);
  process_watcher.start(pid);
  loop.run();
  io_watcher.stop();
  close(fd);
  std::cerr << "shuuryou " << std::endl;
  loop.cleanup();
  return 0;
}
