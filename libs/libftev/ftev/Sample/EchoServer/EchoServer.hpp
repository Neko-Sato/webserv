/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EchoServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 04:11:13 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 06:38:16 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/Stream/StreamConnection.hpp>
#include <ftev/Stream/TCPServer.hpp>

namespace ftev {

class EchoConnection : public StreamConnection {
public:
  EchoConnection(EventLoop &loop, ftpp::Socket &socket);
  ~EchoConnection();

  void on_data(std::vector<char> const &data);
  void on_eof();
  void on_drain();
  void on_error(std::exception const &exce);
  void on_release();
};

class EchoServer : public TCPServer {
public:
  EchoServer(EventLoop &loop, std::string const &host, int port);
  ~EchoServer();

  void on_connect(ftpp::Socket &conn);
};

} // namespace ftev
