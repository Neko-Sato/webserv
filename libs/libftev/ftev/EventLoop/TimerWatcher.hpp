/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TimerWatcher.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/16 05:51:00 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>

#include <ctime>

namespace ftev {

class EventLoop::TimerWatcher : private ftpp::NonCopyable {
public:
  EventLoop &loop;

private:
  bool _isActive;
  TimerWatchers::iterator _it;

  friend class EventLoop;

protected:
  TimerWatcher(EventLoop &loop);

public:
  virtual ~TimerWatcher();

  void start(time_t timeout);
  void cancel();

  virtual void onTimeout() = 0;

  bool getIsActive() const;
};

} // namespace ftev
