/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SignalIOWatcher.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 02:10:31 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/16 14:53:16 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <BaseIOWatcher.hpp>
#include <EventLoop.hpp>
#include <cstddef>
#include <unistd.h>

namespace ftev {

class EventLoop::SignalIOWatcher : public EventLoop::BaseIOWatcher {
private:
  SignalIOWatcher(SignalIOWatcher const &rhs);
  SignalIOWatcher &operator=(SignalIOWatcher const &rhs);

public:
  SignalIOWatcher(EventLoop &loop);
  ~SignalIOWatcher();

  void on_read();
  void on_write();
  void on_error();
};

} // namespace ftev
