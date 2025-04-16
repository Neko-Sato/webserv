/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOWatcher.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 22:09:31 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/16 05:10:58 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>

namespace ftev {

class EventLoop::IOWatcher : private ftpp::NonCopyable {
public:
  EventLoop &loop;

  typedef ftpp::Selector::event_t event_t;

private:
  bool _isActive;
  IOWatchers::iterator _it;

  friend class EventLoop;

protected:
  IOWatcher(EventLoop &loop);

public:
  virtual ~IOWatcher();

  void start(int fd, event_t events);
  void modify(event_t events);
  void stop();

  virtual void onRead() = 0;
  virtual void onWrite() = 0;
  virtual void onExcept() = 0;

  event_t getEvents() const;
  bool getIsActive() const;
};

} // namespace ftev
