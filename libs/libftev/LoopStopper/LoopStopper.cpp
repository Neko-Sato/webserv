/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoopStopper.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:00:41 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/31 23:11:42 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <LoopStopper.hpp>

namespace ftev {

LoopStopper::LoopStopper(ftev::EventLoop &loop) : BaseSignalWatcher(loop) {
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
