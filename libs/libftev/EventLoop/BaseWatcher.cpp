/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseWatcher.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:48:55 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/17 23:37:19 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseWatcher.hpp>

#include <cassert>

namespace ftev {

EventLoop::BaseWatcher::BaseWatcher(EventLoop &loop)
    : loop(loop), _is_active(false) {
  loop._watchers.insert(this);
}

EventLoop::BaseWatcher::~BaseWatcher() {
  assert(!_is_active);
  loop._watchers.erase(this);
}

void EventLoop::BaseWatcher::delete_later() {
  loop._pending_deletion_watchers.push(this);
}

bool EventLoop::BaseWatcher::is_active() const {
  return _is_active;
}

void EventLoop::BaseWatcher::on_release() {
}

} // namespace ftev
