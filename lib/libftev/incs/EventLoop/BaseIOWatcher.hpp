/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseIOWatcher.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:09:31 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/18 00:52:56 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop.hpp>
#include <EventLoop/BaseWatcher.hpp>

namespace ftev {

class EventLoop::BaseIOWatcher : public EventLoop::BaseWatcher {
private:
  typedef ftpp::BaseSelector::event_details event_details;
  bool _is_active;
  IOWatchers::iterator _it;

public:
  BaseIOWatcher(EventLoop &loop);
  virtual ~BaseIOWatcher();
  bool is_active() const;
  void operator()(event_details const &ev);

  void start(int fd, int events);
  void modify(int events);
  void stop();

  int get_fd() const;

  virtual void on_read() = 0;
  virtual void on_write() = 0;
  virtual void on_error() = 0;
};

} // namespace ftev
