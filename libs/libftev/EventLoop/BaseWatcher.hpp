/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseWatcher.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 02:16:25 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/23 19:23:25 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop.hpp>

#include <NonCopyable.hpp>

namespace ftev {

class EventLoop::BaseWatcher : private ftpp::NonCopyable {
public:
  EventLoop &loop;

protected:
  bool _is_active : 1;

  BaseWatcher(EventLoop &loop);

  /*
  It can only be used when allocating with new.
  This is to avoid “use-after-free” when you want to discard in the callback.
  Also, this is the same as delete, which must not be called more than once.
  Using this, on_release is called at the appropriate time, and the opportunity
  for resource release is given by this virtual function.
  */
  void delete_later();

public:
  virtual ~BaseWatcher();

  bool is_active() const;
  virtual void on_release();
};

} // namespace ftev
