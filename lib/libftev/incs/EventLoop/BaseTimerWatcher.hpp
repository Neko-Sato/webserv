/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTimerWatcher.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 02:53:47 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop.hpp>
#include <EventLoop/BaseWatcher.hpp>

#include <ctime>

namespace ftev {

class EventLoop::BaseTimerWatcher : public EventLoop::BaseWatcher {
private:
  bool _is_active;

public:
  BaseTimerWatcher(EventLoop &loop);
  virtual ~BaseTimerWatcher();
  void operator()();

  void start(time_t timeout);
  virtual void on_timeout() = 0;
};

} // namespace ftev
