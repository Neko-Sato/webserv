/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EchoServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 04:11:13 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/16 21:45:19 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/EventLoop/Reaper.hpp>
#include <ftev/Stream/TCPConnection.hpp>
#include <ftev/Stream/TCPServer.hpp>

namespace ftev {

class EchoConnection : public TCPConnection, public EventLoop::Reaper {
public:
  EchoConnection(EventLoop &loop, ftpp::Socket &socket);
  ~EchoConnection();

  void onData(std::vector<char> const &data);
  void onEof();
  void onDrain();
  void onExcept();
  void onRelease();
};

class EchoServer : public TCPServer {
public:
  EchoServer(EventLoop &loop, std::string const &host, int port);
  ~EchoServer();

  void onConnect(ftpp::Socket &conn);
};

} // namespace ftev
