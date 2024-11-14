/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/15 05:14:10 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <selectors/Selector.hpp>

#include <memory>

namespace ftev {

class EventLoop {
private:
  std::auto_ptr<ftpp::BaseSelector> _selector;
  bool _running;
  bool _stop_flag;

  void operator++();

  class TimerWatcher;

  class IOWatcher;
  class SignalWatcher;
  class ProcessWatcher;

public:
  static EventLoop default_loop;

  EventLoop();
  ~EventLoop();

  void run();
  void stop();
};

} // namespace ftev
