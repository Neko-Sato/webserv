/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 01:07:13 by hshimizu          #+#    #+#             */
/*   Updated: 2025/04/01 22:59:38 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/Stream/StreamServer.hpp>

#include <ftpp/noncopyable/NonCopyable.hpp>

#include <list>
#include <string>

namespace ftev {

class TCPServer : public StreamServerProtocol, private ftpp::NonCopyable {
public:
  EventLoop &loop;

private:
  typedef std::list<StreamServerTransport *> Transports;
  Transports _transports;

  TCPServer();

protected:
  TCPServer(EventLoop &loop, const std::string &host, int port);

public:
  ~TCPServer();
};

} // namespace ftev
