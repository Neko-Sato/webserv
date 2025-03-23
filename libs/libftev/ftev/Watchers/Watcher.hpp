/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Watcher.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 02:16:25 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/23 00:16:07 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>

namespace ftev {

class EventLoop::Watcher : private ftpp::NonCopyable {
public:
  EventLoop &loop;

  Watcher();

protected:
  Watcher(EventLoop &loop);

public:
  virtual ~Watcher();

  /*
  It can only be used when allocating with new.
  This is to avoid “use-after-free” when you want to discard in the callback.
  Also, this is the same as delete, which must not be called more than once.
  Using this, on_release is called at the appropriate time, and the opportunity
  for resource release is given by this virtual function.
  */
  void release();
  virtual void on_release();
};

} // namespace ftev
