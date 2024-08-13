/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 20:30:34 by hshimizu          #+#    #+#             */
/*   Updated: 2024/08/13 22:27:04 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "asyncio/EventLoop.hpp"

#include <iostream>
#include <string>

void f(int fd) {
  char buf[1024];
  int n = read(fd, buf, 1024);
  std::cout << fd << ":";
  if (n > 0) {
    buf[n] = 0;
    std::cout << buf << std::endl;
  }
}

#include <fcntl.h>

int main() {
  EventLoop loop;

  int fda = open("aa", O_RDONLY | O_NONBLOCK);
  loop.add_reader(fda, new FunctionHandler<int>(f, fda));
  int fdb = open("bb", O_RDONLY | O_NONBLOCK);
  loop.add_reader(fdb, new FunctionHandler<int>(f, fdb));
  loop.run_forever();
  return 0;
}