/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EventLoop.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:43:33 by hshimizu          #+#    #+#             */
/*   Updated: 2024/11/13 23:23:13 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <selectors/Selector.hpp>

#include <map>

class EventLoop {
private:
  Selector _selector;
  bool _running;
  bool _stop_flag;

  void operator++();

public:
  static EventLoop default_loop;

  EventLoop();
  ~EventLoop();

  void run();
  void stop();
};
