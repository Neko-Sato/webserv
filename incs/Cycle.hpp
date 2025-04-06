/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cycle.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 17:50:51 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/06 20:09:15 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Reader.hpp"
#include "configs/Configs.hpp"
#include "structs/Address.hpp"
#include "structs/Request.hpp"

#include <ftev/Stream/StreamConnection.hpp>
#include <ftpp/noncopyable/NonCopyable.hpp>

class Task;

class Cycle : private ftpp::NonCopyable {
private:
  ftev::StreamConnectionTransport &_transport;
  Configs const &_configs;
  Request const &_request;
  Address const &_address;
  Task *_task;
  Reader *_reader;

  Cycle();

public:
  Cycle(ftev::StreamConnectionTransport &transport, Configs const &configs,
        Request const &request, Address const &address);
  ~Cycle();

  void bufferUpdate(std::deque<char> &buffer, bool eof);
};
