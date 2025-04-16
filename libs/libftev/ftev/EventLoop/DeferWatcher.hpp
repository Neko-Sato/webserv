/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeferWatcher.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/16 05:11:05 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>

namespace ftev {

class EventLoop::DeferWatcher : private ftpp::NonCopyable {
public:
  EventLoop &loop;

private:
  bool _isActive;
  DeferWatchers::iterator _it;

  friend class EventLoop;

protected:
  DeferWatcher(EventLoop &loop);

public:
  virtual ~DeferWatcher();

  void start();
  void cancel();

  virtual void onEvent() = 0;

  bool getIsActive() const;
};

} // namespace ftev
