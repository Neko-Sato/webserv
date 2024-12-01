/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTimerWatcher.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/01 00:07:10 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop.hpp>
#include <EventLoop/BaseWatcher.hpp>

#include <ctime>

namespace ftev {

class EventLoop::BaseTimerWatcher : private EventLoop::BaseWatcher {
private:
  TimerWatchers::iterator _it;

protected:
  BaseTimerWatcher(EventLoop &loop);

public:
  virtual ~BaseTimerWatcher();
  void operator()();
  using BaseWatcher::is_active;

  void start(time_t timeout);
  void cancel();

  virtual void on_timeout() = 0;
};

} // namespace ftev
