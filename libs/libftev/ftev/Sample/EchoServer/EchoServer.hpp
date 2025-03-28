/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EchoServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 04:11:13 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/28 22:38:43 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop/DeferredDelete.hpp>
#include <ftev/Stream/TCPConnection.hpp>
#include <ftev/Stream/TCPServer.hpp>

namespace ftev {

class EchoConnection : public TCPConnection, public EventLoop::DeferredDelete {
public:
  EchoConnection(EventLoop &loop, ftpp::Socket &socket);
  ~EchoConnection();

  void on_data(std::vector<char> const &data);
  void on_eof();
  void on_drain();
  void on_except();
  void on_release();
};

class EchoServer : public TCPServer {
public:
  EchoServer(EventLoop &loop, std::string const &host, int port);
  ~EchoServer();

  void on_connect(ftpp::Socket &conn);
};

} // namespace ftev
