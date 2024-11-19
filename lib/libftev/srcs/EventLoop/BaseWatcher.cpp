/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseWatcher.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:48:55 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/19 21:06:01 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <EventLoop.hpp>
#include <EventLoop/BaseWatcher.hpp>

namespace ftev {

EventLoop::BaseWatcher::BaseWatcher(EventLoop &loop)
    : loop(loop), _is_active(false) {
}

EventLoop::BaseWatcher::~BaseWatcher() {
}

bool EventLoop::BaseWatcher::is_active() const {
  return _is_active;
}

} // namespace ftev