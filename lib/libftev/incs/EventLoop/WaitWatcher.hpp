/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WaitWatcher.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:40:45 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/16 16:58:20 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop/BaseSignalWatcher.hpp>

namespace ftev {

class EventLoop::WaitWatcher : public EventLoop::BaseSignalWatcher {
public:
  WaitWatcher(EventLoop &loop);
  ~WaitWatcher();

  void start();
  void on_signal();
};
} // namespace ftev
