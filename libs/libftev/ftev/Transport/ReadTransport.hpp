/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadTransport.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:36:28 by hshimizu          #+#    #+#             */
/*   Updated: 2025/03/19 01:36:46 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ftev/Protocol/ReadProtocol.hpp>
#include <ftev/Transport/BaseTransport.hpp>

#include <vector>

namespace ftev {

class ReadTransport : virtual public BaseTransport {
private:
  static std::size_t const _chank_size;

  virtual std::size_t _read(char *buffer, std::size_t size) = 0;

protected:
  ReadTransport(EventLoop &loop);

public:
  virtual ~ReadTransport();

  void on_read();

  void start();
  void stop();

  virtual ReadProtocol &getProtocol() = 0;
};

} // namespace ftev
