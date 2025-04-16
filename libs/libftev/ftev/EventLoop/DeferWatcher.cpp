/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeferWatcher.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 05:31:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/16 02:29:41 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/DeferWatcher.hpp>

#include <cassert>

namespace ftev {

EventLoop::DeferWatcher::DeferWatcher(EventLoop &loop)
    : loop(loop), _isActive(false) {
}

EventLoop::DeferWatcher::~DeferWatcher() {
  assert(!_isActive);
}

void EventLoop::DeferWatcher::start() {
  assert(!_isActive);
  _it = loop._deferWatchers.insert(loop._deferWatchers.end(), this);
  _isActive = true;
}

void EventLoop::DeferWatcher::cancel() {
  assert(_isActive);
  loop._deferWatchers.erase(_it);
  _isActive = false;
}

bool EventLoop::DeferWatcher::getIsActive() const {
  return _isActive;
}

} // namespace ftev
