/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTransport.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:29:02 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 20:00:08 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop.hpp>
#include <ftev/EventLoop/BaseIOWatcher.hpp>
#include <ftev/Protocol/BaseProtocol.hpp>

namespace ftev {

class BaseTransport : private ftpp::NonCopyable {
protected:
  class Handler : public ftev::EventLoop::BaseIOWatcher {
  private:
    BaseTransport &_transport;

    Handler();

  public:
    Handler(EventLoop &loop, BaseTransport &_transport);
    ~Handler();

    void on_read();
    void on_write();
    void on_except();
  };

  Handler _handler;

  BaseTransport(EventLoop &loop);

public:
  virtual ~BaseTransport();

  virtual void on_read();
  virtual void on_write();
  virtual void on_except();

  virtual int getFd() const = 0;

  virtual BaseProtocol &getProtocol() = 0;
};

} // namespace ftev
