/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleClient.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 04:36:16 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 06:42:34 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/Stream/TCPConnection.hpp>
#include <ftev/Watchers/TimerWatcher.hpp>

namespace ftev {

class SimpleClient : public TCPConnection {
private:
  class Timer : public EventLoop::TimerWatcher {
  private:
    SimpleClient &_connection;
    std::size_t _count;

  public:
    Timer(EventLoop &loop, SimpleClient &connection);
    ~Timer();

    void on_timeout();
  };

  Timer *_timer;

public:
  SimpleClient(EventLoop &loop, std::string const &host, int port);
  ~SimpleClient();

  void on_data(std::vector<char> const &data);
  void on_eof();
  void on_error(std::exception const &exce);
  void on_drain();
};

} // namespace ftev
