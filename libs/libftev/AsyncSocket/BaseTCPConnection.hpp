/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseTCPConnection.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 23:59:53 by hshimizu          #+#    #+#             */
/*   Updated: 2025/01/11 22:57:23 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <AsyncSocket/BaseAsyncSocket.hpp>

#include <string>
#include <vector>

namespace ftev {

class BaseTCPConnection : public BaseAsyncSocket {
private:
  static std::size_t const _chank_size;
  std::vector<char> _buffer;
  bool _draining : 1;

protected:
  BaseTCPConnection(EventLoop &loop, int domain, int type, int protocol);
  BaseTCPConnection(EventLoop &loop, ftpp::Socket &socket);

public:
  virtual ~BaseTCPConnection();

  void on_write();
  void on_read();

  void write(char const *buffer, size_t size);
  void drain();

  virtual void on_drain() = 0;
  virtual void on_data(std::vector<char> const &data) = 0;
  virtual void on_eof() = 0;
};

} // namespace ftev
