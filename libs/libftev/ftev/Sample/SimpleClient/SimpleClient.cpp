/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 04:36:16 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/16 21:48:37 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Sample/SimpleClient/SimpleClient.hpp>

#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/string/string.hpp>

namespace ftev {

SimpleClient::Timer::Timer(EventLoop &loop,
                           StreamConnectionTransport &transport)
    : EventLoop::TimerWatcher(loop), _transport(transport), _count(0) {
  start(0);
}

SimpleClient::Timer::~Timer() {
  if (getIsActive())
    cancel();
}

void SimpleClient::Timer::onTimeout() {
  std::string tmp = ftpp::Format("Timer: {}") % _count++;
  _transport.write(tmp.data(), tmp.size());
  _transport.drain();
}

SimpleClient::SimpleClient(EventLoop &loop, std::string const &host, int port)
    : TCPConnection(loop, host, port), _timer(NULL) {
  ftpp::logger(ftpp::Logger::INFO,
               ftpp::Format("SimpleClient connected: (host: {}, port: {})") %
                   host % port);
  _timer = new Timer(loop, getTransport());
}

SimpleClient::~SimpleClient() {
  ftpp::logger(ftpp::Logger::INFO, "SimpleClient destroy");
  delete _timer;
}

void SimpleClient::onData(std::vector<char> const &data) {
  ftpp::logger(ftpp::Logger::INFO, ftpp::Format("recv: {}") %
                                       std::string(data.begin(), data.end()));
}

void SimpleClient::onEof() {
  ftpp::logger(ftpp::Logger::INFO, "recv: eof");
}

void SimpleClient::onExcept() {
  ftpp::logger(ftpp::Logger::ERROR, "SimpleClient: error");
  StreamConnectionTransport &transport = getTransport();
  if (_timer->getIsActive())
    _timer->cancel();
  transport.close();
  loop.stop();
}

void SimpleClient::onDrain() {
  _timer->start(1000);
}

} // namespace ftev
