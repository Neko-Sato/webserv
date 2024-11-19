/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseWatcher.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 02:16:25 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/20 02:45:03 by hshimizu         ###   ########.fr       */
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

public:
  BaseWatcher(EventLoop &loop);
  virtual ~BaseWatcher();
  bool is_active() const;
};

} // namespace ftev
