/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WriteTransport.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:36:28 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/18 20:42:55 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/Protocol/WriteProtocol.hpp>
#include <ftev/Transport/BaseTransport.hpp>

#include <vector>

namespace ftev {

class WriteTransport : virtual public BaseTransport {
private:
  std::vector<char> _buffer;
  bool _draining : 1;

  virtual std::size_t _write(char const *buf, std::size_t size) = 0;

protected:
  WriteTransport(EventLoop &loop);

public:
  virtual ~WriteTransport();

  void on_write();

  void write(char const *buffer, std::size_t size);
  void drain();

  virtual WriteProtocol &getProtocol() = 0;
};

} // namespace ftev
