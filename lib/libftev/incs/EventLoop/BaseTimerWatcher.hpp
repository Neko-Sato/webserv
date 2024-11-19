/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTimerWatcher.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/20 02:39:18 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop.hpp>
#include <EventLoop/BaseWatcher.hpp>

#include <ctime>

namespace ftev {

class EventLoop::BaseTimerWatcher : public EventLoop::BaseWatcher {
private:
  using BaseWatcher::_is_active;
  TimerWatchers::iterator _it;

public:
  BaseTimerWatcher(EventLoop &loop);
  virtual ~BaseTimerWatcher();
  void operator()();

  void start(time_t timeout);
  void cancel();

  virtual void on_timeout() = 0;
};

} // namespace ftev
