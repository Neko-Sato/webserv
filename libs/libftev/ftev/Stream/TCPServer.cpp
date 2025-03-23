/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 02:03:41 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 06:26:30 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Stream/TCPServer.hpp>

#include <ftpp/exceptions/OSError.hpp>
#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/macros.hpp>
#include <ftpp/socket/AddrInfos.hpp>
#include <ftpp/socket/Socket.hpp>
#include <ftpp/string/string.hpp>

#include <fcntl.h>

namespace ftev {

TCPServer::Handler::Handler(EventLoop &loop, ftpp::Socket &socket,
                            TCPServer &server)
    : StreamServer(loop, socket), _server(server) {
}

TCPServer::Handler::~Handler() {
}

void TCPServer::Handler::on_connect(ftpp::Socket &conn) {
  try {
    _server.on_connect(conn);
  } catch (std::exception const &e) {
    ftpp::logger(ftpp::Logger::ERROR, ftpp::Format("TCPServer: {}") % e.what());
  }
}

void TCPServer::Handler::on_error(std::exception const &exce) {
  ftpp::logger(ftpp::Logger::ERROR,
               ftpp::Format("TCPServer: {}") % exce.what());
  release();
  _server._handlers.remove(this);
}

void TCPServer::Handler::on_release() {
  delete this;
}

TCPServer::TCPServer(EventLoop &loop, std::string const &host, int port)
    : loop(loop) {
  try {
    ftpp::AddrInfos::Hints hints(AF_UNSPEC, SOCK_STREAM, 0, AI_PASSIVE);
    ftpp::AddrInfos infos(host.c_str(), ftpp::to_string(port).c_str(), hints);
    for (ftpp::AddrInfos::iterator it = infos.begin(); it != infos.end();
         ++it) {
      ftpp::Socket socket(it->ai_family, it->ai_socktype, it->ai_protocol);
      {
        int sockfd = socket.getSockfd();
        int flags = fcntl(sockfd, F_GETFD);
        if (unlikely(flags == -1))
          throw ftpp::OSError(sockfd, "fcntl");
        if (unlikely(fcntl(sockfd, F_SETFD, flags | FD_CLOEXEC) == -1))
          throw ftpp::OSError(sockfd, "fcntl");
        int opt = 1;
        socket.setsockopt(SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
      }
      socket.bind(it->ai_addr, it->ai_addrlen);
      socket.listen();
      Handler *handler = new Handler(loop, socket, *this);
      try {
        _handlers.push_back(handler);
      } catch (...) {
        delete handler;
        throw;
      }
    }
  } catch (...) {
    for (Handlers::iterator it = _handlers.begin(); it != _handlers.end();
         it = _handlers.erase(it))
      delete *it;
    throw;
  }
}

TCPServer::~TCPServer() {
  for (Handlers::iterator it = _handlers.begin(); it != _handlers.end();
       it = _handlers.erase(it))
    delete *it;
}

} // namespace ftev
