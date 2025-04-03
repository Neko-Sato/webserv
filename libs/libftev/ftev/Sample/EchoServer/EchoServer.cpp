/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EchoServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 04:11:13 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/03 15:46:44 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Sample/EchoServer/EchoServer.hpp>

#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>

namespace ftev {

EchoConnection::EchoConnection(EventLoop &loop, ftpp::Socket &socket)
    : TCPConnection(loop, socket), DeferredDelete(loop) {
  ftpp::logger(ftpp::Logger::INFO, "EchoConnection create");
}

EchoConnection::~EchoConnection() {
  ftpp::logger(ftpp::Logger::INFO, "EchoConnection destroy");
}

void EchoConnection::on_data(std::vector<char> const &data) {
  StreamConnectionTransport &transport = get_transport();
  transport.write(data.data(), data.size());
  ftpp::logger(ftpp::Logger::INFO, ftpp::Format("EchoConnection: recv: {}") %
                                       std::string(data.begin(), data.end()));
}

void EchoConnection::on_eof() {
  StreamConnectionTransport &transport = get_transport();
  transport.drain();
}

void EchoConnection::on_drain() {
  StreamConnectionTransport &transport = get_transport();
  transport.close();
  release();
}

void EchoConnection::on_except() {
  StreamConnectionTransport &transport = get_transport();
  transport.close();
  release();
}

void EchoConnection::on_release() {
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

void EchoServer::on_connect(ftpp::Socket &conn) {
  try {
    new EchoConnection(loop, conn);
  } catch (std::exception const &e) {
    ftpp::logger(ftpp::Logger::ERROR,
                 ftpp::Format("EchoServer: {}") % e.what());
  }
}

} // namespace ftev
