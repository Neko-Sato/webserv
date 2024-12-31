/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoopStopper.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:00:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/01 00:44:22 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <LoopStopper/LoopStopper.hpp>

#include <iostream>

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
  std::cerr << "stopping..." << std::endl;
}

} // namespace ftev
