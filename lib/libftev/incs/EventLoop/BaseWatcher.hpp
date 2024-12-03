/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseWatcher.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 02:16:25 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/04 05:24:11 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <EventLoop.hpp>

namespace ftev {

class EventLoop::BaseWatcher {
private:
  BaseWatcher(BaseWatcher const &rhs);
  BaseWatcher &operator=(BaseWatcher const &rhs);

public:
  EventLoop &loop;

protected:
  bool _is_active;

  BaseWatcher(EventLoop &loop);

  /*
  It can only be used when allocating with new.
  This is to avoid “use-after-free” when you want to discard in the callback.
  Also, this is the same as delete, which must not be called more than once.
  */
  void delete_later();

public:
  virtual ~BaseWatcher();

  bool is_active() const;
};

} // namespace ftev
