/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reaper.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 05:37:58 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/16 05:45:56 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/Reaper.hpp>

#include <ftpp/macros.hpp>
#include <ftpp/noncopyable/NonCopyable.hpp>

#include <cassert>

namespace ftev {

EventLoop::Reaper::Reaper(EventLoop &loop) : loop(loop) {
  std::pair<Reapers::iterator, bool> result = loop._reapers.insert(this);
  UNUSED(result);
  assert(result.second);
}

EventLoop::Reaper::~Reaper() {
  loop._reapers.erase(this);
}

void EventLoop::Reaper::release() {
  loop._pendingReapers.push(this);
}

} // namespace ftev
