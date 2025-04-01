/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 04:36:16 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/02 00:08:37 by hshimizu         ###   ########.fr       */
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
  if (is_active())
    cancel();
}

void SimpleClient::Timer::on_timeout() {
  std::string tmp = ftpp::Format("Timer: {}") % _count++;
  _transport.write(tmp.data(), tmp.size());
  _transport.drain();
}

SimpleClient::SimpleClient(EventLoop &loop, std::string const &host, int port)
    : TCPConnection(loop, host, port), _timer(NULL) {
  ftpp::logger(ftpp::Logger::INFO,
               ftpp::Format("SimpleClient connected: (host: {}, port: {})") %
                   host % port);
  _timer = new Timer(loop, get_transport());
}

SimpleClient::~SimpleClient() {
  ftpp::logger(ftpp::Logger::INFO, "SimpleClient destroy");
  delete _timer;
}

void SimpleClient::on_data(std::vector<char> const &data) {
  ftpp::logger(ftpp::Logger::INFO, ftpp::Format("recv: {}") %
                                       std::string(data.begin(), data.end()));
}

void SimpleClient::on_eof() {
  ftpp::logger(ftpp::Logger::INFO, "recv: eof");
}

void SimpleClient::on_except() {
  ftpp::logger(ftpp::Logger::ERROR, "SimpleClient: error");
  StreamConnectionTransport &transport = get_transport();
  if (_timer->is_active())
    _timer->cancel();
  transport.close();
  loop.stop();
}

void SimpleClient::on_drain() {
  _timer->start(1000);
}

} // namespace ftev
