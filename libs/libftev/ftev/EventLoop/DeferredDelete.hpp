/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeferredDelete.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 21:58:01 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/28 22:14:36 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>

namespace ftev {

class EventLoop::DeferredDelete : private ftpp::NonCopyable {
private:
  DeferredDelete();

public:
  EventLoop &loop;

  DeferredDelete(EventLoop &loop);

  void release();
  void on_release();

private:
  virtual ~DeferredDelete();
};

} // namespace ftev
