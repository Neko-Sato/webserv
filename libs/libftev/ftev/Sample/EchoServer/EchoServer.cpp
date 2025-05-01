/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EchoServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 04:11:13 by hshimizu          #+#    #+#             */
/*   Updated: 2025/05/02 02:57:40 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Sample/EchoServer/EchoServer.hpp>

#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>

namespace ftev {

EchoConnection::EchoConnection(EventLoop &loop, ftpp::Socket &socket)
    : TCPConnection(loop, socket), Reaper(loop) {
  ftpp::logger(ftpp::Logger::INFO, "EchoConnection create");
}

EchoConnection::~EchoConnection() {
  ftpp::logger(ftpp::Logger::INFO, "EchoConnection destroy");
}

void EchoConnection::onData(std::vector<char> const &data) {
  StreamConnectionTransport &transport = getTransport();
  transport.write(data.data(), data.size());
  ftpp::logger(ftpp::Logger::INFO, ftpp::Format("EchoConnection: recv: {}") %
                                       std::string(data.begin(), data.end()));
}

void EchoConnection::onEof() {
  StreamConnectionTransport &transport = getTransport();
  transport.drain();
}

void EchoConnection::onDrain() {
  StreamConnectionTransport &transport = getTransport();
  transport.close();
  release();
}

void EchoConnection::onError(std::exception const &exce) {
  ftpp::logger(ftpp::Logger::INFO,
               ftpp::Format("EchoConnection: {}") % exce.what());
  StreamConnectionTransport &transport = getTransport();
  transport.close();
  release();
}

void EchoConnection::onRelease() {
  delete this;
}

EchoServer::EchoServer(EventLoop &loop, std::string const &host, int port)
    : TCPServer(loop, host, port) {
  ftpp::logger(ftpp::Logger::INFO,
               ftpp::Format("EchoServer create: (host: {}, port:{})") % host %
                   port);
}

EchoServer::~EchoServer() {
  ftpp::logger(ftpp::Logger::INFO, "EchoServer destroy");
}

void EchoServer::onConnect(ftpp::Socket &conn) {
  try {
    new EchoConnection(loop, conn);
  } catch (std::exception const &e) {
    ftpp::logger(ftpp::Logger::ERROR,
                 ftpp::Format("EchoServer: {}") % e.what());
  }
}

} // namespace ftev
