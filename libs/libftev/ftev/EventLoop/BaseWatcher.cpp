/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseWatcher.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:48:55 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/22 15:19:24 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/BaseWatcher.hpp>

namespace ftev {

EventLoop::BaseWatcher::BaseWatcher(EventLoop &loop) : loop(loop) {
  loop._watchers.insert(this);
}

EventLoop::BaseWatcher::~BaseWatcher() {
  loop._watchers.erase(this);
}

void EventLoop::BaseWatcher::release() {
  loop._pending_deletion_watchers.push(this);
}

} // namespace ftev
