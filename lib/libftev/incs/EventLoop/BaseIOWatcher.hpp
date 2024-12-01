/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseIOWatcher.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:09:31 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/01 00:07:55 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop.hpp>
#include <EventLoop/BaseWatcher.hpp>

namespace ftev {

class EventLoop::BaseIOWatcher : private EventLoop::BaseWatcher {
public:
  typedef ftpp::BaseSelector::event_t event_t;

private:
  IOWatchers::iterator _it;

  typedef ftpp::BaseSelector::event_details event_details;

protected:
  BaseIOWatcher(EventLoop &loop);

public:
  virtual ~BaseIOWatcher();
  void operator()(event_details const &ev);
  using BaseWatcher::is_active;

  void start(int fd, event_t events);
  void modify(event_t events);
  void stop();

  event_t get_events() const;

  virtual void on_read() = 0;
  virtual void on_write() = 0;
  virtual void on_except() = 0;
};

} // namespace ftev
