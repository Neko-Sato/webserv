/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EchoServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 04:11:13 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 06:38:30 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Sample/EchoServer/EchoServer.hpp>

#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>

namespace ftev {

EchoConnection::EchoConnection(EventLoop &loop, ftpp::Socket &socket)
    : StreamConnection(loop, socket) {
  ftpp::logger(ftpp::Logger::INFO, "EchoConnection create");
}

EchoConnection::~EchoConnection() {
  ftpp::logger(ftpp::Logger::INFO, "EchoConnection destroy");
}

void EchoConnection::on_data(std::vector<char> const &data) {
  write(data.data(), data.size());
  ftpp::logger(ftpp::Logger::INFO, ftpp::Format("EchoConnection: recv: {}") %
                                       std::string(data.begin(), data.end()));
}

void EchoConnection::on_eof() {
  drain();
}

void EchoConnection::on_drain() {
  release();
}

void EchoConnection::on_error(std::exception const &exce) {
  ftpp::logger(ftpp::Logger::ERROR,
               ftpp::Format("EchoConnection: {}") % exce.what());
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
  new EchoConnection(loop, conn);
}

} // namespace ftev
