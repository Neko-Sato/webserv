/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LoopStopper.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:00:41 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/31 23:11:37 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop.hpp>
#include <EventLoop/BaseSignalWatcher.hpp>

namespace ftev {

class LoopStopper : public ftev::EventLoop::BaseSignalWatcher {
public:
  LoopStopper(ftev::EventLoop &loop);
  ~LoopStopper();

  void on_signal();
};

} // namespace ftev
