/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseWatcher.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:48:55 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/06 07:44:42 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseWatcher.hpp>

namespace ftev {

EventLoop::BaseWatcher::BaseWatcher(EventLoop &loop)
    : loop(loop), _is_active(false) {
  loop._watchers.insert(this);
}

EventLoop::BaseWatcher::~BaseWatcher() {
  loop._watchers.erase(this);
}

bool EventLoop::BaseWatcher::is_active() const {
  return _is_active;
}

void EventLoop::BaseWatcher::delete_later() {
  loop._pending_deletion_watchers.push(this);
}

void EventLoop::BaseWatcher::on_release() {
}

} // namespace ftev
