/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EchoServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 04:11:13 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/02 02:55:28 by hshimizu         ###   ########.fr       */
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
  void onError(std::exception const &exce);
  void onRelease();
};

class EchoServer : public TCPServer {
public:
  EchoServer(EventLoop &loop, std::string const &host, int port);
  ~EchoServer();

  void onConnect(ftpp::Socket &conn);
};

} // namespace ftev
