/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOWatcher.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:09:31 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/23 00:18:07 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/Watchers/Watcher.hpp>

namespace ftev {

class EventLoop::IOWatcher : public EventLoop::Watcher {
private:
  bool _is_active;
  IOWatchers::iterator _it;

protected:
  IOWatcher(EventLoop &loop);

public:
  typedef ftpp::Selector::event_t event_t;

  virtual ~IOWatcher();
  void operator()(event_t events);

  event_t get_events() const;
  void start(int fd, event_t events);
  void modify(event_t events);
  void stop();

  virtual void on_read() = 0;
  virtual void on_write() = 0;
  virtual void on_except() = 0;

  bool is_active() const;
};

} // namespace ftev
