/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoopStopper.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:00:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/23 00:19:00 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/LoopStopper/LoopStopper.hpp>

#include <iostream>

namespace ftev {

LoopStopper::LoopStopper(ftev::EventLoop &loop) : SignalWatcher(loop) {
  start(SIGINT);
}

LoopStopper::~LoopStopper() {
  if (is_active())
    stop();
}

void LoopStopper::on_signal() {
  loop.stop();
}

} // namespace ftev
