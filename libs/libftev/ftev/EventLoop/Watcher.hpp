/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Watcher.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 02:16:25 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/28 21:57:37 by hshimizu         ###   ########.fr       */
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
};

} // namespace ftev
