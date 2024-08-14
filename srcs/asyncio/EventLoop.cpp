/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 21:29:29 by hshimizu          #+#    #+#             */
/*   Updated: 2024/08/14 18:20:34 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asyncio/EventLoop.hpp"

EventLoop::EventLoop() {
  __running = false;
}

EventLoop::~EventLoop() {
  for (std::map<int, IOHandlers>::iterator it = __fd_ready.begin();
       it != __fd_ready.end(); ++it) {
    delete it->second.read;
    delete it->second.write;
  }
  while (!__ready.empty()) {
    BaseHandler *handler = __ready.front();
    __ready.pop();
    delete handler;
  }
}

void EventLoop::call_soon(BaseHandler *handler) {
  __ready.push(handler);
}

void EventLoop::add_reader(int fd, BaseHandler *handler) {
  IOHandlers *fd_ready = &__fd_ready[fd];

  if (!fd_ready->read && !fd_ready->write)
    __selector.add(fd, Selector::READ);
  else {
    int events = Selector::READ;
    if (fd_ready->write)
      events |= Selector::WRITE;
    __selector.modify(fd, events);
  }
  fd_ready->read = handler;
}

void EventLoop::remove_reader(int fd) {
  IOHandlers *fd_ready = &__fd_ready[fd];

  if (!fd_ready->write) {
    __selector.remove(fd);
    delete fd_ready->read;
    __fd_ready.erase(fd);
  } else {
    __selector.modify(fd, Selector::WRITE);
    delete fd_ready->read;
    fd_ready->read = NULL;
  }
}

void EventLoop::add_writer(int fd, BaseHandler *handler) {
  IOHandlers *fd_ready = &__fd_ready[fd];

  if (!fd_ready->read && !fd_ready->write)
    __selector.add(fd, Selector::WRITE);
  else {
    int events = Selector::WRITE;
    if (fd_ready->read)
      events |= Selector::READ;
    __selector.modify(fd, events);
  }
  fd_ready->write = handler;
}

void EventLoop::remove_writer(int fd) {
  IOHandlers *fd_ready = &__fd_ready[fd];

  if (!fd_ready->read) {
    __selector.remove(fd);
    delete fd_ready->write;
    __fd_ready.erase(fd);
  } else {
    __selector.modify(fd, Selector::READ);
    delete fd_ready->write;
    fd_ready->write = NULL;
  }
}

void EventLoop::stop() {
  __running = false;
}

void EventLoop::run_forever() {
  __running = true;
  try {
    while (__running)
      (*this)++;
  } catch (...) {
    __running = false;
    throw;
  }
  __running = false;
}

void EventLoop::operator++(int) {
  {
    std::vector<BaseSelector::events> events;
    __selector.wait(events, __ready.empty() ? -1 : 0);
    for (std::vector<BaseSelector::events>::iterator ev = events.begin();
         ev != events.end(); ++ev) {
      IOHandlers *fd_ready = &__fd_ready[ev->fd];
      if (ev->events & BaseSelector::READ)
        __ready.push(fd_ready->read->copy());
      if (ev->events & BaseSelector::WRITE)
        __ready.push(fd_ready->write->copy());
    }
  }
  while (!__ready.empty()) {
    BaseHandler *handler = __ready.front();
    __ready.pop();
    try {
      (*handler)();
    } catch (...) {
      delete handler;
      throw;
    }
    delete handler;
  }
}
