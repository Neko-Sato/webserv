/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2024/08/13 22:02:44 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __ASYNCIO_EVENTLOOP_HPP__
#define __ASYNCIO_EVENTLOOP_HPP__

#include <map>
#include <queue>

#include "asyncio/Handler.hpp"
#include "selectors/Selector.hpp"

class EventLoop {
private:
  struct IOHandlers {
    BaseHandler *read;
    BaseHandler *write;
  };

  bool __running;
  Selector __selector;
  std::map<int, IOHandlers> __fd_ready;
  std::queue<BaseHandler *> __ready;

public:
  EventLoop();
  ~EventLoop();

  void call_soon(BaseHandler *handler);
  void add_reader(int fd, BaseHandler *handler);
  void remove_reader(int fd);
  void add_writer(int fd, BaseHandler *handler);
  void remove_writer(int fd);
  void stop();
  void run_forever();
  EventLoop &operator++(int);
};

#endif
