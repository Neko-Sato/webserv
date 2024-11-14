/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 17:57:51 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/15 05:14:36 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <cassert>

#include <memory>

namespace ftev {

EventLoop EventLoop::default_loop;

EventLoop::EventLoop()
    : _selector(new ftpp::Selector), _running(false), _stop_flag(true) {
}

EventLoop::~EventLoop() {
}

void EventLoop::operator++() {
  typedef typename std::deque<ftpp::BaseSelector::events> Events;
  _running = true;
  Events events;
  _selector->select(events, 0);
  for (Events::iterator it = events.begin(); it != events.end(); ++it) {
  }
  _running = false;
}

void EventLoop::run() {
  assert(!_running);
  _stop_flag = false;
  while (!_stop_flag)
    operator++();
}

void EventLoop::stop() {
  _stop_flag = true;
}

} // namespace ftev