/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MixinWriter.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 23:59:53 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/02 07:36:43 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <AsyncSocket/BaseAsyncSocket.hpp>

#include <vector>

namespace ftev {

class MixinWriter : virtual public BaseAsyncSocket {
private:
  std::vector<char> _buffer;
  bool _draining;

protected:
  MixinWriter();

public:
  virtual ~MixinWriter();
  void on_write();

  void write(char const *buffer, size_t size);
  void drain();

  virtual void on_drain() = 0;
};

} // namespace ftev
