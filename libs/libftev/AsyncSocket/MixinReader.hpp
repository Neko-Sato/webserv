/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MixinReader.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hshimizu <hshimizu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 23:59:53 by hshimizu          #+#    #+#             */
/*   Updated: 2024/12/02 07:15:48 by hshimizu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <AsyncSocket/BaseAsyncSocket.hpp>

#include <deque>

namespace ftev {

class MixinReader : virtual public BaseAsyncSocket {
private:
  static std::size_t const _chank_size = 1024;

  std::deque<char> _buffer;

protected:
  MixinReader();

public:
  virtual ~MixinReader();
  void on_read();

  virtual void on_data(std::deque<char> &data) = 0;
  virtual void on_eof(std::deque<char> &data) = 0;
};

} // namespace ftev
