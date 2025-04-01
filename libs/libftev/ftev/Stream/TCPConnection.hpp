/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPConnection.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 01:47:05 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/02 08:21:58 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/Stream/StreamConnection.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>

#include <string>

namespace ftev {

class TCPConnection : public StreamConnectionProtocol,
                      private ftpp::NonCopyable {
public:
  EventLoop &loop;

private:
  StreamConnectionTransport *_transport;

  TCPConnection();

protected:
  TCPConnection(EventLoop &loop, std::string const &host, int port);
  TCPConnection(EventLoop &loop, ftpp::Socket &socket);

public:
  virtual ~TCPConnection();

  StreamConnectionTransport &get_transport();
};

} // namespace ftev
