/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 04:36:16 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/24 06:53:42 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ftev/Sample/SimpleClient/SimpleClient.hpp>

#include <ftpp/format/Format.hpp>
#include <ftpp/logger/Logger.hpp>
#include <ftpp/string/string.hpp>

namespace ftev {

SimpleClient::Timer::Timer(EventLoop &loop, SimpleClient &connection)
    : EventLoop::TimerWatcher(loop), _connection(connection), _count(0) {
  start(0);
}

SimpleClient::Timer::~Timer() {
  if (is_active())
    cancel();
}

void SimpleClient::Timer::on_timeout() {
  std::string tmp = ftpp::Format("Timer: {}") % _count++;
  Handler &handler = _connection.getHandler();
  handler.write(tmp.data(), tmp.size());
  handler.drain();
}

SimpleClient::SimpleClient(EventLoop &loop, std::string const &host, int port)
    : TCPConnection(loop, host, port), _timer(NULL) {
  _timer = new Timer(loop, *this);
}

SimpleClient::~SimpleClient() {
  delete _timer;
}

void SimpleClient::on_data(std::vector<char> const &data) {
  ftpp::logger(ftpp::Logger::INFO, ftpp::Format("recv: {}") %
                                       std::string(data.begin(), data.end()));
}

void SimpleClient::on_eof() {
  ftpp::logger(ftpp::Logger::INFO, "recv: eof");
}

void SimpleClient::on_error(std::exception const &exce) {
  if (_timer->is_active())
    _timer->cancel();
  ftpp::logger(ftpp::Logger::ERROR,
               ftpp::Format("SimpleClient: {}") % exce.what());
  loop.stop();
}

void SimpleClient::on_drain() {
  _timer->start(1000);
}

} // namespace ftev
