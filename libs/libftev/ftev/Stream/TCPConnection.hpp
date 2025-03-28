/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPConnection.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 02:03:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/28 22:45:00 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/Stream/StreamConnection.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>

#include <string>

namespace ftev {

class TCPConnection : private ftpp::NonCopyable {
private:
  class Handler : public StreamConnection {
  private:
    TCPConnection &_connection;

  public:
    Handler(EventLoop &loop, ftpp::Socket &socket, TCPConnection &connection);
    ~Handler();

    void on_data(std::vector<char> const &data);
    void on_eof();
    void on_drain();
    void on_except();
  };

public:
  EventLoop &loop;

private:
  Handler *_handler;

  TCPConnection();

protected:
  TCPConnection(EventLoop &loop, std::string const &host, int port);
  TCPConnection(EventLoop &loop, ftpp::Socket &socket);

public:
  virtual ~TCPConnection();

  StreamConnection &getHandler();
  virtual void on_data(std::vector<char> const &data) = 0;
  virtual void on_eof() = 0;
  virtual void on_except() = 0;
  virtual void on_drain() = 0;
};

} // namespace ftev
