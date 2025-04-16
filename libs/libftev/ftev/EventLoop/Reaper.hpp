/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Reaper.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 05:37:58 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/16 21:46:49 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>

namespace ftev {

class EventLoop::Reaper : private ftpp::NonCopyable {
public:
  EventLoop &loop;

  Reaper(EventLoop &loop);
  virtual ~Reaper();

  void release();

  virtual void onRelease() = 0;
};

} // namespace ftev
