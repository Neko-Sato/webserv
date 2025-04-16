/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoopStopper.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:00:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/16 05:51:38 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/LoopStopper/LoopStopper.hpp>

#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>

#include <iostream>

namespace ftev {

LoopStopper::LoopStopper(ftev::EventLoop &loop) : SignalWatcher(loop) {
  start(SIGINT);
}

LoopStopper::~LoopStopper() {
  if (getIsActive())
    stop();
}

void LoopStopper::onSignal() {
  ftpp::logger(ftpp::Logger::INFO, ftpp::Format("stopping..."));
  loop.stop();
}

} // namespace ftev
