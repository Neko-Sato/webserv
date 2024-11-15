/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTimerWatcher.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/15 22:15:48 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop.hpp>
#include <ctime>

namespace ftev {

class EventLoop::BaseTimerWatcher {
protected:
  EventLoop &_loop;

private:
  BaseTimerWatcher(BaseTimerWatcher const &rhs);
  BaseTimerWatcher &operator=(BaseTimerWatcher const &rhs);

public:
  BaseTimerWatcher(EventLoop &loop);
  virtual ~BaseTimerWatcher();
  void operator()();

  void start(time_t timeout);
  virtual void onTimeout() = 0;
};

} // namespace ftev
