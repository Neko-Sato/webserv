#include <EventLoop.hpp>
#include <Watchers/IOWatcher.hpp>
#include <Watchers/ProcessWatcher.hpp>
#include <Watchers/SignalWatcher.hpp>
#include <Watchers/TimerWatcher.hpp>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

void on_timeout(ftev::TimerWatcher<int> &watcher, int _) {
  (void)_;
  std::cout << "Timer fired!" << std::endl;
  watcher.start(1000);
}

void on_read(ftev::IOWatcher<int> &watcher, int _) {
  (void)_;
  char buf[10];
  ssize_t size = read(watcher.get_fd(), buf, 10);
  if (size == 0) {
    watcher.close();
    watcher.loop.stop();
    return;
  }
  size = write(1, buf, size);
}

void on_signal(ftev::SignalWatcher<int> &watcher, int _) {
  (void)_;
  (void)watcher;
  std::cout << "Signal received!" << std::endl;
}

void on_exit(ftev::ProcessWatcher<int> &watcher, int status, int _) {
  (void)_;
  (void)watcher;
  std::cout << "Process exited! " << status << std::endl;
}

#include <unistd.h>

int main() {
  pid_t pid = fork();
  if (pid == 0) {
    char const *const argv[] = {"python", "-m", "http.server", NULL};
    execve("/bin/python", const_cast<char *const *>(argv), environ);
  }
  ftev::EventLoop &loop = ftev::EventLoop::default_loop;

  ftev::TimerWatcher<int> timer_watcher(loop, on_timeout, 0);
  ftev::IOWatcher<int> io_watcher(loop, on_read, NULL, NULL, 0);
  ftev::SignalWatcher<int> signal_watcher(loop, on_signal, 0);
  ftev::ProcessWatcher<int> process_watcher(loop, on_exit, 0);

  timer_watcher.start(1000);
  io_watcher.start(open("./pipe", O_RDONLY), ftpp::BaseSelector::READ);
  signal_watcher.start(SIGINT);
  process_watcher.start(pid);
  loop.run();
  return 0;
}
