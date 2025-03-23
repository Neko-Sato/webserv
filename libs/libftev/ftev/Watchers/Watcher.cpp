/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Watcher.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:48:55 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 05:16:51 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/EventLoop.hpp>
#include <ftev/Watchers/Watcher.hpp>

namespace ftev {

EventLoop::Watcher::Watcher(EventLoop &loop) : loop(loop) {
  loop._watchers.insert(this);
}

EventLoop::Watcher::~Watcher() {
  loop._watchers.erase(this);
}

void EventLoop::Watcher::release() {
  loop._pending_deletion_watchers.push(this);
}

void EventLoop::Watcher::on_release() {
}

} // namespace ftev
