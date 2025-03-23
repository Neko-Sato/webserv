/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerWatcher.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/23 00:16:49 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/Watchers/Watcher.hpp>

#include <ctime>

namespace ftev {

class EventLoop::TimerWatcher : public EventLoop::Watcher {
private:
  bool _is_active;
  TimerWatchers::iterator _it;

protected:
  TimerWatcher(EventLoop &loop);

public:
  virtual ~TimerWatcher();
  void operator()();

  void start(time_t timeout);
  void cancel();

  virtual void on_timeout() = 0;

  bool is_active() const;
};

} // namespace ftev
