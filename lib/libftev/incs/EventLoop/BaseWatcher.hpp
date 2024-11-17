/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseWatcher.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 02:16:25 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/17 20:42:17 by hshimizu         ###   ########.fr       */
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

  BaseWatcher(EventLoop &loop);
  virtual ~BaseWatcher();
  virtual bool is_active() const = 0;
};

} // namespace ftev
