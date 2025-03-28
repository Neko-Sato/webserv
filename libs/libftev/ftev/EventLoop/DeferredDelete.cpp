/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeferredDelete.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 21:58:01 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/28 22:29:13 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/EventLoop/DeferredDelete.hpp>

namespace ftev {

EventLoop::DeferredDelete::DeferredDelete(EventLoop &loop) : loop(loop) {
  loop._deferred_deletes.insert(this);
}

EventLoop::DeferredDelete::~DeferredDelete() {
  loop._deferred_deletes.erase(this);
}

void EventLoop::DeferredDelete::release() {
  loop._pending_deletes.push(this);
}

} // namespace ftev
