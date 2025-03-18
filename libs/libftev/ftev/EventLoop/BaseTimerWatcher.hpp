/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTimerWatcher.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 19:03:37 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/BaseWatcher.hpp>

#include <ctime>

namespace ftev {

class EventLoop::BaseTimerWatcher : public EventLoop::BaseWatcher {
private:
  using BaseWatcher::_is_active;
  TimerWatchers::iterator _it;

protected:
  BaseTimerWatcher(EventLoop &loop);

public:
  virtual ~BaseTimerWatcher();
  void operator()();

  void start(time_t timeout);
  void cancel();

  virtual void on_timeout() = 0;
};

} // namespace ftev
