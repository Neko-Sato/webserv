/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoopStopper.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:00:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/28 22:15:01 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/SignalWatcher.hpp>

namespace ftev {

class LoopStopper : private EventLoop::SignalWatcher {
public:
  LoopStopper(ftev::EventLoop &loop);
  ~LoopStopper();

  void on_signal();
};

} // namespace ftev
