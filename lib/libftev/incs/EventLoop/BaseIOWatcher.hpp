/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseIOWatcher.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:09:31 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/16 17:06:06 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop.hpp>
#include <selectors/BaseSelector.hpp>

namespace ftev {

class EventLoop::BaseIOWatcher {
protected:
  EventLoop &_loop;

private:
  typedef ftpp::BaseSelector::event_detals event_detals;
  int _fd;

  BaseIOWatcher(BaseIOWatcher const &rhs);
  BaseIOWatcher &operator=(BaseIOWatcher const &rhs);

public:
  BaseIOWatcher(EventLoop &loop, int fd);
  virtual ~BaseIOWatcher();
  void on_event(event_detals const &ev);

  void start(int events);
  void modify(int events);
  void close();

  int get_fd() const;

  virtual void on_read() = 0;
  virtual void on_write() = 0;
  virtual void on_error() = 0;
};

} // namespace ftev
