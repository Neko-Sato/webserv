/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTransport.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:29:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/19 02:23:13 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/BaseIOWatcher.hpp>
#include <ftev/Protocol/BaseProtocol.hpp>

namespace ftev {

class BaseTransport : private ftev::EventLoop::BaseIOWatcher {
protected:
  BaseTransport(EventLoop &loop);

  BaseIOWatcher &_getWatcher();

public:
  virtual ~BaseTransport();

  void on_except();
  void on_release();

  using BaseWatcher::delete_later;
  virtual int getFd() const = 0;
  virtual BaseProtocol &getProtocol() = 0;
};

} // namespace ftev
