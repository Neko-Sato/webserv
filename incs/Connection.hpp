/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 23:06:24 by hshimizu          #+#    #+#             */
/*   Updated: 2025/02/05 12:32:19 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

#include <AsyncSocket/BaseTCPConnection.hpp>
#include <urllib/URI.hpp>

#include <map>
#include <string>

class Connection : public ftev::BaseTCPConnection {
public:
  enum State { ERROR, REQUEST, HEADER, RESPONSE };

  typedef std::map<std::string, std::vector<std::string> > Headers;

private:
  Server const &_server;
  State _state;
  std::deque<char> _buffer;

  std::string _method;
  ftpp::URI _uri;
  std::string _version;
  Headers _headers;

  void _parseRequest(std::string const &line);
  void _parseHeader(std::string const &line);
  void _makeResponseTask();
  void _sendResponse();

public:
  Connection(ftev::EventLoop &loop, ftpp::Socket &socket,
                 Server const &server);
  ~Connection();

  void on_data(std::vector<char> const &data);
  void on_eof();
  void on_drain();
  void on_except();
  void on_release();
};
