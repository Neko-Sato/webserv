/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cycle.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 17:50:51 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/16 22:45:21 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Reader.hpp"
#include "configs/Configs.hpp"
#include "structs/Address.hpp"
#include "structs/Request.hpp"

#include <ftev/EventLoop/DeferWatcher.hpp>
#include <ftev/Stream/StreamConnection.hpp>
#include <ftpp/noncopyable/NonCopyable.hpp>

class Cycle : private ftpp::NonCopyable {
private:
  ftev::StreamConnectionTransport &_transport;
  Request const &_request;
  Address const &_address;
  Location::Task *_task;
  Reader *_reader;
  bool _keepAlive;

  Cycle();

public:
  Cycle(ftev::StreamConnectionTransport &transport,
        ftev::EventLoop::DeferWatcher &complete, Configs const &configs,
        Request const &request, Address const &address);
  ~Cycle();

  void bufferUpdate(std::deque<char> &buffer, bool bufferClosed);
  bool getKeepAlive() const;
};
