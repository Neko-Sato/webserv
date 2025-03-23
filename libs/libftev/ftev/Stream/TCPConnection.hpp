/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPConnection.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 02:03:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 06:33:43 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/Stream/StreamConnection.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>

#include <string>

namespace ftev {

class TCPConnection : private ftpp::NonCopyable {
public:
  class Handler : public StreamConnection {
  private:
    TCPConnection &_connection;

  public:
    Handler(EventLoop &loop, ftpp::Socket &socket, TCPConnection &connection);
    ~Handler();

    void on_data(std::vector<char> const &data);
    void on_eof();
    void on_drain();
    void on_error(std::exception const &exce);
    void on_release();
  };

  EventLoop &loop;

private:
  Handler *_handler;

  TCPConnection();

public:
  TCPConnection(EventLoop &loop, std::string const &host, int port);
  virtual ~TCPConnection();

  Handler &getHandler();
  virtual void on_data(std::vector<char> const &data) = 0;
  virtual void on_eof() = 0;
  virtual void on_error(std::exception const &exce) = 0;
  virtual void on_drain() = 0;
};

} // namespace ftev
