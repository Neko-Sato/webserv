/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleClient.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 04:36:16 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/16 21:47:45 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop/TimerWatcher.hpp>
#include <ftev/Stream/TCPConnection.hpp>

namespace ftev {

class SimpleClient : public TCPConnection {
private:
  class Timer : public EventLoop::TimerWatcher {
  private:
    StreamConnectionTransport &_transport;
    std::size_t _count;

  public:
    Timer(EventLoop &loop, StreamConnectionTransport &transport);
    ~Timer();

    void onTimeout();
  };

  Timer *_timer;

public:
  SimpleClient(EventLoop &loop, std::string const &host, int port);
  ~SimpleClient();

  void onData(std::vector<char> const &data);
  void onEof();
  void onDrain();
  void onExcept();
};

} // namespace ftev
